/*
 * File:   utilities.h
 * Author: Erik
 *
 * Created on den 9 januari 2016, 22:56
 */

#ifndef DEBUG_UTIL_H
#define	DEBUG_UTIL_H

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
#define DEBUG_UTIL_CHAR_BUFFER_SIZE (256u)
extern const char NEWLINE[];
extern const char WARNING_TAG[];
extern const char ERROR_TAG[];

// =============================================================================
// Global variable declarations
// =============================================================================

// Alround buffer for temporary calculations.
// Do not use this buffer in an interrupt rutine or trust other functions
// not to modify its contents.
extern char g_debug_util_char_buffer[DEBUG_UTIL_CHAR_BUFFER_SIZE];

// =============================================================================
// Public function declarations
// =============================================================================

#ifdef	__cplusplus
}
#endif

#endif	/* DEBUG_UTIL_H */

