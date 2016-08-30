// =============================================================================
// Include statements
// =============================================================================
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "midi_file.h"
#include "uart.h"
#include "debug_util.h"

// =============================================================================
// Private type definitions
// =============================================================================

//
// Do not store the entire MIDI file in RAM since it migth be big.
// Use a fifo buffer instead, and read more data from the SD card as needed.
//
#define MIDI_FILE_BUFFER_SIZE   (1024u)
typedef struct midi_file_buffer_t
{
    uint8_t buffer[MIDI_FILE_BUFFER_SIZE];
    size_t first;
    size_t last;
    size_t size;
    size_t file_pos;
} midi_file_buffer_t;

// =============================================================================
// Global variables
// =============================================================================

// =============================================================================
// Private constants
// =============================================================================

static uint8_t midi_test_file[] =
{
	0x4d,	0x54,	0x68,	0x64,	0x0,	0x0,	0x0,	0x6,
	0x0,	0x0,	0x0,	0x1,	0x1,	0xe0,	0x4d,	0x54,
	0x72,	0x6b,	0x0,	0x0,	0x0,	0x4e,	0x0,	0xff,
	0x3,	0x4,	0x66,	0x69,	0x6c,	0x65,	0x0,	0xff,
	0x51,	0x3,	0x7,	0xa1,	0x20,	0x0,	0xff,	0x58,
	0x4,	0x4,	0x2,	0x18,	0x8,	0x0,	0x90,	0x48,
	0x64,	0x0,	0x91,	0x4c,	0x64,	0x8e,	0x7e,	0x80,
	0x48,	0x40,	0x0,	0x81,	0x4c,	0x40,	0x2,	0x90,
	0x4a,	0x64,	0x0,	0x91,	0x4e,	0x64,	0x8e,	0x7e,
	0x80,	0x4a,	0x40,	0x0,	0x81,	0x4e,	0x40,	0x2,
	0x90,	0x4c,	0x64,	0x0,	0x91,	0x50,	0x64,	0x8e,
	0x7e,	0x80,	0x4c,	0x40,	0x0,	0x81,	0x50,	0x40,
	0x0,	0xff,	0x2f,	0x0,	0xff,	0xff, 	0xff,	0xff
};

// =============================================================================
// Private variables
// =============================================================================

static midi_file_buffer_t file_buffer;

// =============================================================================
// Private function declarations
// =============================================================================

/**
 * @brief Loads a midi file from midi data in an array.
 * @param test_file - the midi data to load the file fom
 * @param test_file_size - number of bytes test_file.
 */
static void load_test_file(const uint8_t test_file[], size_t test_file_size);

/**
 * @brief Uses the uart interface to print the error.
 */
static void print_buffer_overflow_error(void);

// =============================================================================
// Public function definitions
// =============================================================================

void midi_file_open(char* file_name)
{
#ifdef DEBUG
    sprintf(g_debug_util_char_buffer, "Opening midi file \"%s\"", file_name);
    uart_write_string(g_debug_util_char_buffer);
#endif

    // TODO Load midi file from SD card
    memset(&file_buffer, 0x00, sizeof(file_buffer));
    load_test_file(midi_test_file, sizeof(midi_test_file));
}

bool midi_file_append(uint8_t data)
{
    bool overflow = false;

    if (file_buffer.size >= MIDI_FILE_BUFFER_SIZE)
    {
        uart_write_string("midi file buffer overflow in midi_file_append");
        uart_write_string(NEWLINE);
        overflow = true;
    }
    else
    {
        if (0 != file_buffer.size)
        {
            if (++file_buffer.last == MIDI_FILE_BUFFER_SIZE)
            {
                file_buffer.last = 0;
            }
        }

        file_buffer.buffer[file_buffer.last] = data;
        file_buffer.size++;
        file_buffer.file_pos++;
    }

    return !overflow;
}

bool midi_file_append_chunk(uint8_t data_array[], size_t number_of_bytes)
{
    bool overflow  = false;
    size_t new_last_index;

    if (MIDI_FILE_BUFFER_SIZE - file_buffer.size >= number_of_bytes)
    {
        uart_write_string("midi file buffer overflow in midi_file_append_chunk");
        uart_write_string(NEWLINE);
        overflow = true;
    }
    else
    {
        //
        // Place the first bytes in the buffer at
        // indicies [last, MIDI_FILE_BUFFER_SIZE - 1].
        // Then place the last byte from index 0 and forth
        //
        // Example where 6 bytes are added to a size 10 buffer.
        // Before:
        //                first    last
        // [0,  1,  2,  3,  4,  5,  6,  7,  8,  9]
        //
        // After:
        //         last   first
        // [0,  1,  2,  3,  4,  5,  6,  7,  8,  9]
        //  <------->                   <------->
        // second chunk                first chunk
        //

        //
        // Chunk to place in file_buffer at [last, MIDI_FILE_BUFFER_SIZE - 1]
        //
        size_t first_chunk_size = MIDI_FILE_BUFFER_SIZE - file_buffer.last - 1;
        size_t first_chunk_first_index;
        bool second_chunk_needed;

        if (0 != file_buffer.size)
        {
            if (MIDI_FILE_BUFFER_SIZE - file_buffer.last - 1 < number_of_bytes)
            {
                first_chunk_size = MIDI_FILE_BUFFER_SIZE - file_buffer.last - 1;
                new_last_index = MIDI_FILE_BUFFER_SIZE - 1;
                second_chunk_needed = true;
            }
            else
            {
                first_chunk_size = number_of_bytes;
                new_last_index = file_buffer.last + first_chunk_size;
                second_chunk_needed = false;
            }
            
            first_chunk_first_index = file_buffer.last + 1;
        }
        else
        {
            if (MIDI_FILE_BUFFER_SIZE - file_buffer.last < number_of_bytes)
            {
                first_chunk_size = MIDI_FILE_BUFFER_SIZE - file_buffer.last;
                new_last_index = MIDI_FILE_BUFFER_SIZE - 1;
                second_chunk_needed = true;
            }
            else
            {
                first_chunk_size = number_of_bytes;
                new_last_index = file_buffer.last + first_chunk_size - 1;
                second_chunk_needed = false;
            }

            first_chunk_first_index = file_buffer.last;
        }

        memcpy(&file_buffer.buffer[first_chunk_first_index],
               &data_array[0],
               first_chunk_size);

        if (second_chunk_needed)
        {
            //
            // Chunk to place in file_buffer from index 0 and forth
            //
            size_t second_chunk_size = number_of_bytes - first_chunk_size;

            memcpy(&file_buffer.buffer[0],
                   &data_array[first_chunk_size],
                   second_chunk_size);

            new_last_index = second_chunk_size - 1;
        }

        file_buffer.last = new_last_index;
        file_buffer.size += number_of_bytes;
        file_buffer.file_pos += number_of_bytes;
    }

    return !overflow;
}

uint8_t midi_file_get(void)
{
    uint8_t byte_to_return = 0xFF;

    if (midi_file_has_next())
    {
        byte_to_return = file_buffer.buffer[file_buffer.first++];

        if (file_buffer.first == MIDI_FILE_BUFFER_SIZE)
        {
            file_buffer.first = 0;
        }

        file_buffer.size--;
    }

    return byte_to_return;
}

uint8_t midi_file_peek(void)
{
    return file_buffer.buffer[file_buffer.first];
}

bool  midi_file_has_next(void)
{
    return (0 != file_buffer.size);
}

// =============================================================================
// Private function definitions
// =============================================================================

static void load_test_file(const uint8_t test_file[], size_t test_file_size)
{
    size_t i;

    if (test_file_size > MIDI_FILE_BUFFER_SIZE)
    {
        test_file_size = MIDI_FILE_BUFFER_SIZE;
        uart_write_string("midi file buffer overflow in load_test_file");
        uart_write_string(NEWLINE);
    }
    else
    {
        for (i = 0; i != test_file_size; ++i)
        {
            file_buffer.buffer[i] = test_file[i];
        }

        file_buffer.first = 0;
        file_buffer.last = test_file_size - 1;
        file_buffer.size = test_file_size;
        file_buffer.file_pos = test_file_size - 1;
    }
}