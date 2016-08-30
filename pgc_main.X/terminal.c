// =============================================================================
// Include statements
// =============================================================================
#include <xc.h>

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "terminal.h"
#include "terminal_help.h"
#include "debug_util.h"
#include "uart.h"
#include "event_queue.h"
#include "spi.h"

// =============================================================================
// Private type definitions
// =============================================================================

// =============================================================================
// Global variables
// =============================================================================

// =============================================================================
// Private constants
// =============================================================================
static const char TERMINAL_PASSWORD[] = "open terminal";
#define PASSWORD_STR_LEN (13u)

#define CMD_BUFFER_SIZE (128)
static char cmd_buffer[CMD_BUFFER_SIZE] = {0};
static const char COMMAND_TERMINATION_CHARACTER = '\n';

static const char TERMINAL_OPEN[]   = "\tTerminal is now open.\r\n";
static const char COMMAND_ENTER[]   = "Command: ";
static const char SYNTAX_ERROR[]    = "\t[Syntax error]\r\n";
static const char CMD_HELP[]        = "help";

static const char SET[]             = "set ";
static const char GET[]             = "get ";

//
// Commands
//

/*§
 Closes down the terminal.
 */
static const char CMD_EXIT[]            = "exit";

/*§
 Forces a software reset.
 */
static const char CMD_SYSTEM_RESET[]    = "system reset\r\n";

/*§
 Runs the spi3 initialization code.
 */
static const char CMD_INIT_SPI3[]       = "spi3 init";

/*§
 Sends a 32 bit value over the spi3 interface.
 Parameters: <dword to send (in hex)>
 */
static const char CMD_SEND_SPI3_DWORD[]   = "spi3 send dword";

//
// Get commands
//

/*§
 Displays the registers values of the spi3 module.
 */
static const char GET_SPI3_STATUS[]       = "get spi3 status";

/*§
 Displays the registers values of the spi4 module.
 */
static const char GET_SPI4_STATUS[]       = "get spi4 status";

// =============================================================================
// Private variables
// =============================================================================

static bool command_received_event = false;
static bool terminal_open = false;
static char* reply_buff;

// =============================================================================
// Private function declarations
// =============================================================================

/**
 * @brief Executes the command stored in the command buffer.
 * @param arg - not used
 * @return always 0
 */
int execute_command(int arg);

/* **********************************************
 *  Commands
 ***********************************************/
// Get commands

// Set commands

// Commands

// =============================================================================
// Public function definitions
// =============================================================================

int terminal_handle_uart_event(int arg)
{
    uint16_t bytes_received;
    uint16_t index;
    bool password_ok;
    size_t i;

    bytes_received = uart_get_receive_buffer_size();


    if (terminal_open)
    {
        //
        // Terminal is open
        //
        uart_disable_rx_interrupt();

        if (bytes_received >= CMD_BUFFER_SIZE)
        {
            uart_clear_receive_buffer();
            bytes_received = 0;
        }
        else
        {
            for (i = 0; i != bytes_received; ++i)
            {
                cmd_buffer[i] = uart_get(i);
            }

            cmd_buffer[i] = 0;
        }

        if (NULL != strchr(cmd_buffer, COMMAND_TERMINATION_CHARACTER))
        {
            event_queue_push_callback(&execute_command,
                                      EVENT_QUEUE_NO_ARG,
                                      EVENT_PRIO_LOW);
        }

        uart_enable_rx_interrupt();
    }
    else
    {
        //
        // Terminal is closed
        //
        if (bytes_received >= PASSWORD_STR_LEN)
        {
            index = 0;
            password_ok = true;

            while ( (true == password_ok) &&
                    (index != PASSWORD_STR_LEN) )
            {
                if (uart_get(bytes_received - 1 - index) !=
                    TERMINAL_PASSWORD[PASSWORD_STR_LEN - 1 - index])
                {
                    password_ok = false;
                }

                ++index;
            }

            if (password_ok)
            {
                terminal_open = true;
                uart_clear_receive_buffer();

                uart_write_string(NEWLINE);
                uart_write_string(TERMINAL_OPEN);
                uart_write_string(COMMAND_ENTER);
            }
        }
    }

    return 0;
}

// =============================================================================
// Private function definitions
// =============================================================================

int execute_command(int arg)
{
    bool syntax_error = false;
    char reply_buffer[128];
    reply_buff = reply_buffer;

    if (NULL != strstr(cmd_buffer, GET))
    {
        //
        // GET
        //
        if (NULL != strstr(cmd_buffer, GET_SPI3_STATUS))
        {
            spi_print_debug_status(SPI_DEVICE_DSP);
        }
        else if (NULL != strstr(cmd_buffer, GET_SPI4_STATUS))
        {
            spi_print_debug_status(SPI_DEVICE_SDCARD);
        }
        else
        {
            syntax_error = true;
        }
    }
    else if (NULL != strstr(cmd_buffer, SET))
    {
        //
        // SET
        //
        syntax_error = true;
    }
    else
    {
        //
        // CMD
        //
        if (NULL != strstr(cmd_buffer, CMD_HELP))
        {
            terminal_help(cmd_buffer);
        }
        else if (NULL != strstr(cmd_buffer, CMD_EXIT))
        {
            terminal_open = false;

            uart_write_string("\tTerminal closed.");
            uart_write_string(NEWLINE);
        }
        else if (NULL != strstr(cmd_buffer, CMD_SYSTEM_RESET))
        {
            // assume interrupts are disabled
            // assume the DMA controller is suspended
            // assume the device is locked
            // perform a system unlock sequence
            // starting critical sequence
            SYSKEY = 0x00000000; //write invalid key to force lock
            SYSKEY = 0xAA996655; //write key1 to SYSKEY
            SYSKEY = 0x556699AA; //write key2 to SYSKEY
            // OSCCON is now unlocked
            // set SWRST bit to arm reset
            RSWRSTSET = 1;
            // read RSWRST register to trigger reset
            volatile uint32_t dummy;
            dummy = RSWRST;
            // prevent any unwanted code execution until reset occurs
            while(1);
        }
        else if (NULL != strstr(cmd_buffer, CMD_SEND_SPI3_DWORD))
        {
            // TODO implement CMD_SEND_SPI3_DWORD func
            spi_write_dword(SPI_DEVICE_DSP, 0x50505050);
        }
        else if (NULL != strstr(cmd_buffer, CMD_INIT_SPI3))
        {
            spi_init(SPI_DEVICE_DSP);
        }
        else
        {
            syntax_error = true;
        }
    }

    if (syntax_error)
    {
        uart_clear_receive_buffer();
        uart_write_string(SYNTAX_ERROR);
    }

    if (terminal_open)
    {
        uart_clear_receive_buffer();
        uart_write_string(COMMAND_ENTER);
    }

    return 0;
}

/* *******************************************************
 * Command help functions
 *********************************************************/
