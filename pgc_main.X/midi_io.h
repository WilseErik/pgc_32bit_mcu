#ifndef MIDI_IO_H
#define	MIDI_IO_H

#ifdef	__cplusplus
//extern "C" {
#endif

// =============================================================================
// Include statements
// =============================================================================

// =============================================================================
// Public type definitions
// =============================================================================

// =============================================================================
// Global constatants
// =============================================================================

// =============================================================================
// Global variable declarations
// =============================================================================

// =============================================================================
// Public function declarations
// =============================================================================

/**
 * @brief Opens a midi file.
 * @details This is an asynchronous operation.
 * @param file_name - the midi file to open.
 */
void midi_io_file_open(char* file_name);

/**
 * @brief Appends one byte to the midi file.
 * @param data - byte to append.
 */
void midi_io_file_append(uint8_t data);

/**
 * @brief Appends an array of bytes to the midi file.
 * @param data_array - array of bytes to append.
 * @param number_of_bytes - number of bytes in data_array.
 */
void midi_io_file_append_chunk(uint8_t data_array[], size_t number_of_bytes);

/**
 * 
 * @return 
 */
uint8_t midi_io_file_get(void);

uint8_t midi_io_file_peek(void);

#ifdef	__cplusplus
}
#endif

#endif	/* MIDI_IO_H */

