#ifndef MIDI_FILE_H
#define	MIDI_FILE_H

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
void midi_file_open(char* file_name);

/**
 * @brief Appends one byte to the midi file.
 * @param data - byte to append.
 * @return true if the data was appended successfully.
 */
bool midi_file_append(uint8_t data);

/**
 * @brief Appends an array of bytes to the midi file.
 * @param data_array - array of bytes to append.
 * @param number_of_bytes - number of bytes in data_array.
 * @return true if the data was appended successfully.
 */
bool midi_file_append_chunk(uint8_t data_array[], size_t number_of_bytes);

/**
 * @brief Gets the next byte from the midi file.
 * @return The next byte.
 */
uint8_t midi_file_get(void);

/**
 * @brief Peaks at the next byte in the midi file.
 * @return The next byte.
 */
uint8_t midi_file_peek(void);

/**
 * @brief Checks if there is one more character to get.
 * @return true if there is one more character in the file buffer.
 */
bool  midi_file_has_next(void);

#ifdef	__cplusplus
}
#endif

#endif	/* MIDI_FILE_H */

