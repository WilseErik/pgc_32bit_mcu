/*
 * References:
 * - PIC32 Family Reference Manual,
 *   Section 14. Timers, document number DS61105F.
 *
 */



// =============================================================================
// Include statements
// =============================================================================
#include <stdint.h>
#include <stdbool.h>

#include <xc.h>

#include "wait_timer.h"
#include "mcu.h"

// =============================================================================
// Private type definitions
// =============================================================================

// =============================================================================
// Global variables
// =============================================================================

// =============================================================================
// Private constants
// =============================================================================
const uint32_t TIMER_FREQ = 1000000;

// =============================================================================
// Private variables
// =============================================================================
static bool timer_is_initialized = false;

// =============================================================================
// Private function declarations
// =============================================================================

// =============================================================================
// Public function definitions
// =============================================================================

void wait_timer_us(uint16_t us_to_wait)
{
    //
    // - 16 bit timer
    // - Internal perpheral clock as source
    // - 1:1 Prescaler
    //
    T2CON = 0;
    PR2 = PBCLK_FREQ_HZ / TIMER_FREQ;
    TMR2 = 0x0000;
    IEC0bits.T2IE = 0;  // Do not use interrupts since this is a blocking func.
    IFS0bits.T2IF = 0;

    T2CONbits.ON = 1;

    while (0 != us_to_wait)
    {
        if (IFS0bits.T2IF)
        {
            IFS0bits.T2IF = 0;
            --us_to_wait;
        }
    }

    T2CONbits.ON = 0;
    IFS0bits.T2IF = 0;
}

void wait_timer_ms(uint16_t ms_to_wait)
{
    while (0 != ms_to_wait--)
    {
        wait_timer_us(1000);
    }
}


// =============================================================================
// Private function definitions
// =============================================================================