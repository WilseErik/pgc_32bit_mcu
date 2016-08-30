// =============================================================================
// Include statements
// =============================================================================
#include <xc.h>

#include <stdint.h>

#include "uart.h"
#include "debug_util.h"

// =============================================================================
// Private type definitions
// =============================================================================

// =============================================================================
// Global variables
// =============================================================================

// =============================================================================
// Private constants
// =============================================================================

static char separator[] =
"================================================================================";

// =============================================================================
// Private variables
// =============================================================================

// =============================================================================
// Private function declarations
// =============================================================================

/**
 * @brief Checks why the startup reason and prints it over the uart interface.
 * @param void
 * @return void
 */
void check_startup_reason(void);

// =============================================================================
// Public function definitions
// =============================================================================

void mcu_init(void)
{
    // Interrupt controller configured for multi vectored mode
    INTCONbits.MVEC = 1;
    
    check_startup_reason();
}

// =============================================================================
// Private function definitions
// =============================================================================

void check_startup_reason(void)
{
    uint32_t reset_reason;

    uart_init();

    uart_write_string(NEWLINE);
    uart_write_string(NEWLINE);
    uart_write_string(separator);
    uart_write_string(NEWLINE);
    uart_write_string("PIC32 started.");
    uart_write_string(NEWLINE);
    uart_write_string("Reset reason was:");
    uart_write_string(NEWLINE);

    reset_reason = RCON;

    if (reset_reason & _RCON_POR_MASK)
    {
        uart_write_string("\t[POR] Power On Reset");
        uart_write_string(NEWLINE);
        RCONbits.POR = 0;
    }

    if (reset_reason & _RCON_BOR_MASK)
    {
        uart_write_string("\t[BOR] Brown out Reset");
        uart_write_string(NEWLINE);
        RCONbits.BOR = 0;
    }

    if (reset_reason & _RCON_IDLE_MASK)
    {
        uart_write_string("\t[IDLE] Wakeup from idle mode");
        uart_write_string(NEWLINE);
        RCONbits.IDLE = 0;
    }

    if (reset_reason & _RCON_SLEEP_MASK)
    {
        uart_write_string("\t[SLEEP] Wakeup from sleep mode");
        uart_write_string(NEWLINE);
        RCONbits.SLEEP = 0;
    }

    if (reset_reason & _RCON_WDTO_MASK)
    {
        uart_write_string("\t[WDTO] Watchdog timeout");
        uart_write_string(NEWLINE);
        RCONbits.WDTO = 0;
    }

    if (reset_reason & _RCON_DMTO_MASK)
    {
        uart_write_string("\t[DMTO] Deadman Timer timeout");
        uart_write_string(NEWLINE);
    }

    if (reset_reason & _RCON_SWR_MASK)
    {
        uart_write_string("\t[SWR] Software Reset");
        uart_write_string(NEWLINE);
        RCONbits.SWR = 0;
    }

    if (reset_reason & _RCON_EXTR_MASK)
    {
        uart_write_string("\t[EXTR] External Reset (MCLR)");
        uart_write_string(NEWLINE);
        RCONbits.EXTR = 0;
    }

    if (reset_reason & _RCON_CMR_MASK)
    {
        uart_write_string("\t[CMR] Configuration Mismatch Reset");
        uart_write_string(NEWLINE);
        RCONbits.CMR = 0;
    }

    if (reset_reason & _RCON_BCFGFAIL_MASK)
    {
        uart_write_string("\t[BCFGFAIL] An error occurred during a read of the");
        uart_write_string("\t           primary and alternate configuration registers");
        uart_write_string(NEWLINE);
        RCONbits.BCFGFAIL = 0;
    }

    if (reset_reason & _RCON_BCFGERR_MASK)
    {
        uart_write_string("\t[BCFGFAIL] An error occurred during a read of the");
        uart_write_string("\t           primary configuration registers");
        uart_write_string(NEWLINE);
        RCONbits.BCFGERR = 0;
    }

    uart_write_string(separator);
    uart_write_string(NEWLINE);
}

