/*
 * Implementation references:
 *  - PIC32 users guide - Section 42 Oscillators with Enhanced PLL,
 *    document number DS60001250A
 *  - PIC32 users guide - Section 21 UART, document number DS61107F
 *
 */


// =============================================================================
// Include statements
// =============================================================================

#include <xc.h>
#include <sys/attribs.h>

#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "uart.h"
#include "mcu.h"
#include "pinmap.h"
#include "event_queue.h"
#include "terminal.h"

// =============================================================================
// Private type definitions
// =============================================================================

// =============================================================================
// Global variables
// =============================================================================

// =============================================================================
// Private constants
// =============================================================================
#define BUFFER_SIZE     ((uint16_t)1024)
#define BACKSPACE_CHAR  (0x08)

static const uint32_t UART_BAUD = 9600;

// =============================================================================
// Private variables
// =============================================================================
static bool uart_initialized = false;

// Implement the TX and RX buffers as circular buffers:
static volatile uint8_t rx_buff[BUFFER_SIZE];
static volatile uint8_t tx_buff[BUFFER_SIZE];

static volatile uint16_t rx_buff_first = 0;
static volatile uint16_t rx_buff_last = 0;
static volatile uint16_t tx_buff_first = 0;
static volatile uint16_t tx_buff_last = 0;

static volatile uint16_t rx_buff_size = 0;
static volatile uint16_t tx_buff_size = 0;

// =============================================================================
// Private function declarations
// =============================================================================

/**
 * @brief Starts transmission of the tx buffer.
 * @param void
 * @return void
 */
static void start_tx(void);

// =============================================================================
// Public function definitions
// =============================================================================

void uart_init()
{
    volatile int32_t wait_cnt = 0;

    if (false == uart_initialized)
    {
        //
        // Variables
        //
        rx_buff_first = 0;
        rx_buff_last = 0;
        tx_buff_first = 0;
        tx_buff_last = 0;

        rx_buff_size = 0;
        tx_buff_size = 0;

        //
        // IO ports
        //
        UART_TX_PIN_DIRECTION = DIRECTION_OUTPUT;
        UART_RX_PIN_DIRECTION = DIRECTION_INPUT;

        // Unlock the Peripheral Pin Select registers
        // For reference, see document DS60001250A - page 42-26
        //__builtin_disable_interrupts();

        SYSKEY = 0x0;
        SYSKEY = 0xAA996655;
        SYSKEY = 0x556699AA;
        CFGCONbits.IOLOCK = 0;

        UART_TX_PPS_REGISTER = UART_TX_PPS_VALUE;
        UART_RX_PPS_REGISTER = UART_RX_PPS_VALUE;

        // Lock the Peripheral Pin Select registers
        //__builtin_enable_interrupts();

        CFGCONbits.IOLOCK = 1;
        SYSKEY = 0x0;

        //
        // UART module
        //
        U1MODE = 0x00000000;
        U1STA = 0x00000000;

        U1BRG = (PBCLK_FREQ_HZ / UART_BAUD) / 16 - 1;

        U1MODEbits.PDSEL = 0; // 8 bit data, no parity
        U1MODEbits.STSEL = 0; // 1 Stop bit

        // Interrupt is generated and asserted while the transmit buffer
        // is empty. Therefore, disable TX interrupt when there is nothing
        // more to send.
        U1STAbits.UTXISEL0 = 0;
        U1STAbits.UTXISEL1 = 1;
        IPC28bits.U1TXIP = 2;  // Interrupt priority
        IFS3bits.U1TXIF = 0;
        IEC3bits.U1TXIE = 0;   // TX interrupt enable

        // Interrupt flag bit is asserted while receive buffer is not empty
        U1STAbits.URXISEL = 0;
        
        IPC28bits.U1RXIP = 2;  // Interrupt priority
        IFS3bits.U1RXIF = 0;
        IEC3bits.U1RXIE = 1;   // RX interrupt enable

        U1MODEbits.UARTEN = 1;
        U1STAbits.UTXEN = 1;
        U1STAbits.URXEN = 1;

        for (wait_cnt = 0; wait_cnt != PBCLK_FREQ_HZ / UART_BAUD; ++wait_cnt)
        {
            ;
        }

        __builtin_enable_interrupts();

        uart_initialized = true;
    }
}

void uart_write(uint8_t data)
{
    if ((0 == tx_buff_size) && (0 == U1STAbits.UTXBF))
    {
        // hw transmit buffer not full but tx buffer is.
        U1TXREG = data;
    }
    else if (tx_buff_size < BUFFER_SIZE)
    {
        if (0 != tx_buff_size)
        {
            ++tx_buff_last;

            if (tx_buff_last >= BUFFER_SIZE)
            {
                tx_buff_last = 0;
            }
        }

        tx_buff[tx_buff_last] = data;

        ++tx_buff_size;

        uart_enable_tx_interrupt();
    }
}

void uart_write_string(const char* data)
{
    const uint8_t* p = (const uint8_t*)data;

    // Update the tx buffer.
    while (*p && (tx_buff_size < BUFFER_SIZE))
    {
        if (0 != tx_buff_size)
        {
            ++tx_buff_last;

            if (tx_buff_last >= BUFFER_SIZE)
            {
                tx_buff_last = 0;
            }
        }

        tx_buff[tx_buff_last] = *(p++);

        ++tx_buff_size;
    }

    start_tx();
}

void uart_write_array(uint16_t nbr_of_bytes, const uint8_t* data)
{
    uint16_t i;

    // Update the tx buffer.
    for (i = 0; i != nbr_of_bytes; ++i)
    {
        if (tx_buff_size < BUFFER_SIZE)
        {
            if (0 != tx_buff_size)
            {
                ++tx_buff_last;

                if (tx_buff_last >= BUFFER_SIZE)
                {
                    tx_buff_last = 0;
                }
            }

            tx_buff[tx_buff_last] = *(data++);

            if (tx_buff_size < BUFFER_SIZE)
            {
                ++tx_buff_size;
            }
        }
    }

    start_tx();
}

uint8_t uart_get(uint16_t index)
{
    uint16_t i;
    uint8_t data;

    uart_disable_rx_interrupt();

    i = rx_buff_first + index;

    if (i >= BUFFER_SIZE)
    {
        i -= BUFFER_SIZE;
    }

    data = rx_buff[i];

    uart_enable_rx_interrupt();

    return data;
}

uint16_t uart_get_receive_buffer_size(void)
{
    return rx_buff_size;
}

bool uart_is_receive_buffer_empty(void)
{
    return (0 == rx_buff_size);
}

void uart_clear_receive_buffer(void)
{
    uart_disable_rx_interrupt();

    rx_buff_size = 0;
    rx_buff_first = 0;
    rx_buff_last = 0;

    uart_enable_rx_interrupt();
}


// =============================================================================
// Private function definitions
// =============================================================================

void __ISR(_UART1_RX_VECTOR, ipl2) uart1_rx_isr(void)
{
    uint8_t received;

    //
    // RX
    //
    if (IFS3bits.U1RXIF)
    {
        uart_disable_tx_interrupt();

        if (U1STAbits.OERR)
        {
            U1STAbits.OERR = 0;
        }

        while (U1STAbits.URXDA)
        {
            received = U1RXREG;

            if (BACKSPACE_CHAR != received)
            {
                if (0 != rx_buff_size)
                {
                    ++rx_buff_last;

                    if (rx_buff_last >= BUFFER_SIZE)
                    {
                        rx_buff_last = 0;
                    }
                }

                rx_buff[rx_buff_last] = received;

                ++rx_buff_size;
                uart_write(received);
            }
            else
            {
                if (1 < rx_buff_size)
                {

                    if (0 != rx_buff_last)
                    {
                        --rx_buff_last;
                    }
                    else
                    {
                        rx_buff_last = BUFFER_SIZE - 1;
                    }
                }

                if (0 != rx_buff_size)
                {
                    --rx_buff_size;
                    uart_write(received);
                }
            }
        }

        event_queue_push_callback(&terminal_handle_uart_event,
                                  EVENT_QUEUE_NO_ARG,
                                  EVENT_PRIO_LOW
                                  );

        uart_enable_tx_interrupt();

        IFS3bits.U1RXIF = 0;
    }
}

void __ISR(_UART1_TX_VECTOR, ipl2) uart1_tx_isr(void)
{
    uint8_t received;

    //
    // TX
    //
    if (IFS3bits.U1TXIF)
    {
        while ((0 == U1STAbits.UTXBF) && (0 != tx_buff_size))
        {
            // TX fifo not full and there are more things to send
            U1TXREG = tx_buff[tx_buff_first];

            if (1 != tx_buff_size)
            {
                ++tx_buff_first;
            }

            if (tx_buff_first >= BUFFER_SIZE)
            {
                tx_buff_first = 0;
            }

            --tx_buff_size;
        }

        if (0 == tx_buff_size)
        {
            uart_disable_tx_interrupt();
        }

        IFS3bits.U1TXIF = 0;
    }
}

static void start_tx(void)
{
    uart_disable_tx_interrupt();
    uart_disable_rx_interrupt();

    while ((0 != tx_buff_size) && (0 == U1STAbits.UTXBF))
    {
        U1TXREG = tx_buff[tx_buff_first];

        if (1 != tx_buff_size)
        {
            ++tx_buff_first;
        }

        if (tx_buff_first >= BUFFER_SIZE)
        {
            tx_buff_first = 0;
        }

        --tx_buff_size;
    }

    uart_enable_tx_interrupt();
    uart_enable_rx_interrupt();
}

