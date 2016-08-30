#ifndef MIDI_DEFS_H
#define	MIDI_DEFS_H

#ifdef	__cplusplus
//extern "C" {
#endif

// =============================================================================
// Include statements
// =============================================================================
#include <stdint.h>

// =============================================================================
// Public type definitions
// =============================================================================


#define MIDI_EVENT_EVENT_MASK			(0xF0)
#define MIDI_EVENT_CHANNEL_MASK			(0x0F)

typedef enum
{
	MIDI_EVENT_NOTE_OFF 		= 0x80,
	MIDI_EVENT_NOTE_ON 		= 0x90,
	MIDI_EVENT_NOTE_AFTERTOUCH 	= 0xA0,
	MIDI_EVENT_CONTROLLER 		= 0xB0,
	MIDI_EVENT_PROGRAM_CHANGE 	= 0xC0,
	MIDI_EVENT_CHANNEL_AFTERTOUCH	= 0xD0,
	MIDI_EVENT_PITCH_BEND		= 0xE0,
	MIDI_EVENT_META_EVENT		= 0xF0
} midi_event_t;

typedef enum
{
	MIDI_META_EV_SEQUENCE_NUMBER		= 0x00,
	MIDI_META_EV_TEXT_EVENT			= 0x01,
	MIDI_META_EV_COPYRIGHT_NOTICE		= 0x02,
	MIDI_META_EV_TRACK_NAME			= 0x03,
	MIDI_META_EV_INSTRUMENT_NAME		= 0x04,
	MIDI_META_EV_LYRICS			= 0x05,
	MIDI_META_EV_MARKER			= 0x06,
	MIDI_META_EV_CUE_POINT			= 0x07,
	MIDI_META_EV_MIDI_CHANNEL_PREFIX	= 0x20,
	MIDI_META_EV_END_OF_TRACK		= 0x2F,
	MIDI_META_EV_SET_TEMPO			= 0x51,
	MIDI_META_EV_SMPTE_OFFSET		= 0x54,
	MIDI_META_EV_TIME_SIGNATURE		= 0x58,
	MIDI_META_EV_KEY_SIGNATURE		= 0x59,
	MIDI_META_EV_EOF_MIDI			= 0xFF
} midi_meta_event_t;


// =============================================================================
// Global constatants
// =============================================================================

// =============================================================================
// Global variable declarations
// =============================================================================

// =============================================================================
// Public function declarations
// =============================================================================

#ifdef	__cplusplus
}
#endif

#endif	/* MIDI_DEFS_H */

