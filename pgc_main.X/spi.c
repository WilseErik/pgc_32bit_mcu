/*
 * This file forms a hardware abstraction layer for the spi peripherals.
 *
 * References:
 * - PIC32 Family Reference Manual,
 *   Section 23. Serial Peripheral Interface (SPI), document number DS61106G.
 *
 * Used SPI modules:
 * - SPI2 by the GPU
 * - SPI3 by the audio DSP interface
 * - SPI4 by the SD card
 */


// =============================================================================
// Include statements
// =============================================================================
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include <xc.h>
#include <sys/attribs.h>

#include "spi.h"
#include "mcu.h"
#include "pinmap.h"
#include "uart.h"
#include "debug_util.h"

// =============================================================================
// Private type definitions
// =============================================================================

#define SPI_WRITE_BUFFER_SIZE (64u)

typedef struct transmit_fifo_t
{
    uint32_t buffer[SPI_WRITE_BUFFER_SIZE];
    uint32_t first;
    uint32_t last;
    uint32_t size;
} transmit_fifo_t;

// =============================================================================
// Global variables
// =============================================================================

// =============================================================================
// Private constants
// =============================================================================
static const uint32_t SPI3_BAUD = 1000000;

// Use a lower baud when initializing the SD card
static const uint32_t SPI4_BAUD = 200000;

// =============================================================================
// Private variables
// =============================================================================
static bool spi3_initialized = false;
static bool spi4_initialized = false;

// Circular FIFO write buffer
transmit_fifo_t spi3_transmit_fifo;

// =============================================================================
// Private function declarations
// =============================================================================

/**
 * @brief Initializes the SPI3 module and sets it up for communicating
 *        with the audio DSP.
 * @param void
 * @return void
 */
static void spi3_initialize(void);

/**
 * @brief Sends a 32 it word over the spi3 module.
 * @details This is an asynchronous operation.
 * @return void
 */
static void spi3_write(uint32_t data);

/**
 * @brief Sends several 32 bit values using the spi3 module
 * @param v - array with the values to send.
 * @param number_of_elements - number of elements to send.
 */
static void spi3_write_vector(uint32_t* v, uint32_t number_of_elements);

/**
 * @brief Starts the transmission of the SPI3 tx FIFO buffer.
 * @return void
 */
static void spi3_start_tx(void);

/**
 * @brief Masks out the spi3 tx interrupt.
 */
static inline void spi3_disable_tx_interrupt(void)
{
    IEC4bits.SPI3TXIE = 0;
}

/**
 * @brief Enables the spi3 tx interrupt.
 */
static inline void spi3_enable_tx_interrupt(void)
{
    IEC4bits.SPI3TXIE = 1;
}

/**
 * @brief Initializes the spi4 module for the SD card inteface.
 */
static void spi4_initialize();

/**
 * @brief Sends and recieves one byte over the spi4 interface.
 * @param data_to_send - the byte to send.
 * @return The byte received from the spi transaction.
 */
uint8_t spi4_tranceive_blocking(uint8_t data_to_send);

// =============================================================================
// Public function definitions
// =============================================================================

void spi_init(spi_device_t spi_device)
{
    switch (spi_device)
    {
    case SPI_DEVICE_DSP:
        spi3_initialize();
        break;

    case SPI_DEVICE_SDCARD:
        spi4_initialize();
        
    default:
        break;
    }
}

uint8_t spi_byte_tranceive_blocking(spi_device_t spi_device,
                                    uint8_t data_to_send)
{
    uint8_t returned_byte = 0x00;

    switch (spi_device)
    {
    case SPI_DEVICE_SDCARD:
        returned_byte = spi4_tranceive_blocking(data_to_send);

    default:
        break;
    }

    return returned_byte;
}

void spi_write_dword(spi_device_t spi_device, uint32_t data)
{
    switch (spi_device)
    {
    case SPI_DEVICE_DSP:
        spi3_write(data);
        break;

    default:
        break;
    }
}

void spi_write_dword_vect(spi_device_t spi_device,
                          uint32_t data[],
                          uint32_t number_of_elements)
{
    switch (spi_device)
    {
    case SPI_DEVICE_DSP:
        spi3_write_vector(data, number_of_elements);
        break;

    default:
        break;
    }
}

void spi_update_sd_card_baud(uint32_t new_baud)
{
    while (SPI4STATbits.SPIBUSY)
    {
        ;   // Wait for any operation to complete before turning the module off.
    }
    SPI4CONbits.ON = 0;

    // Set the baud rate
    SPI4BRG = (PBCLK_FREQ_HZ / new_baud) / 2 - 1;

    SPI4CONbits.ON = 1;
}

void spi_print_debug_status(spi_device_t spi_device)
{
    switch (spi_device)
    {
    case SPI_DEVICE_DSP:
        sprintf(g_debug_util_char_buffer, "\tSPI3CON: %X", SPI3CON);
        uart_write_string(g_debug_util_char_buffer);
        uart_write_string(NEWLINE);

        sprintf(g_debug_util_char_buffer, "\tSPI3CON2: %X", SPI3CON2);
        uart_write_string(g_debug_util_char_buffer);
        uart_write_string(NEWLINE);

        sprintf(g_debug_util_char_buffer, "\tSPI3STAT: %X", SPI3STAT);
        uart_write_string(g_debug_util_char_buffer);
        uart_write_string(NEWLINE);

        sprintf(g_debug_util_char_buffer, "\tSPI3BRG: %X", SPI3BRG);
        uart_write_string(g_debug_util_char_buffer);
        uart_write_string(NEWLINE);
        break;

    case SPI_DEVICE_SDCARD:
        sprintf(g_debug_util_char_buffer, "\tSPI4CON: %X", SPI4CON);
        uart_write_string(g_debug_util_char_buffer);
        uart_write_string(NEWLINE);

        sprintf(g_debug_util_char_buffer, "\tSPI4CON2: %X", SPI4CON2);
        uart_write_string(g_debug_util_char_buffer);
        uart_write_string(NEWLINE);

        sprintf(g_debug_util_char_buffer, "\tSPI4STAT: %X", SPI4STAT);
        uart_write_string(g_debug_util_char_buffer);
        uart_write_string(NEWLINE);

        sprintf(g_debug_util_char_buffer, "\tSPI4BRG: %X", SPI4BRG);
        uart_write_string(g_debug_util_char_buffer);
        uart_write_string(NEWLINE);
        break;

    default:
        break;
    }
}

// =============================================================================
// Private function definitions
// =============================================================================

void __ISR(_SPI3_TX_VECTOR, ipl4) spi3_isr(void)
{
    volatile uint32_t dummy;

    IFS4bits.SPI3TXIF = 0;
    dummy = SPI3BUF;

    if (0 != spi3_transmit_fifo.size)
    {
        while ((!SPI3STATbits.SPITBF) && (0 != spi3_transmit_fifo.size))
        {
            SPI3BUF = spi3_transmit_fifo.buffer[spi3_transmit_fifo.first];
            spi3_transmit_fifo.size--;

            if (0 != spi3_transmit_fifo.size)
            {
                spi3_transmit_fifo.first++;

                if (SPI_WRITE_BUFFER_SIZE == spi3_transmit_fifo.first)
                {
                    spi3_transmit_fifo.first = 0;
                }
            }
        }
    }
    else
    {
        spi3_disable_tx_interrupt();
    }
}

/*
 * From PIC32 Family Reference Manual,
 * Section 23. Serial Peripheral Interface (SPI), document number DS61106G,
 * page 18
 *
 * Perform the following steps to set up the SPI module for the Master mode operation:
 * 1.Disable the SPI interrupts in the respective IECx register.
 * 2.Stop and reset the SPI module by clearing the ON bit.
 * 3.Clear the receive buffer.
 * 4.Clear the ENHBUF bit (SPIxCON<16>) if using Standard Buffer mode or set the bit if using Enhanced Buffer mode.
 * 5.If SPI interrupts are not going to be used, skip this step and continue to step 5. Otherwise the following additional steps are performed:
 * a) Clear the SPIx interrupt flags/events in the respective IFSx register.
 * b) Write the SPIx interrupt priority and subpriority bits in the respective IPCx register.
 * c) Set the SPIx interrupt enable bits in the respective IECx register.
 * 6.Write the Baud Rate register, SPIxBRG.
 * 7.Clear the SPIROV bit (SPIxSTAT<6>).
 * 8.Write the desired settings to the SPIxCON register with MSTEN (SPIxCON<5>) = 1.
 * 9.Enable SPI operation by setting the ON bit (SPIxCON<15>).
 * 10.Write the data to be transmitted to the SPIxBUF register. Transmission (and reception) will start as soon as data is written to the SPIxBUF register.
 */
static void spi3_initialize(void)
{
    volatile uint32_t dummy = 0;

    if (false == spi3_initialized)
    {
        memset(&spi3_transmit_fifo, 0, sizeof(transmit_fifo_t));

        //
        // IO ports
        //
        AUDIO_MISO_PIN_DIRECTION = DIRECTION_INPUT;
        AUDIO_MOSI_PIN = DIRECTION_OUTPUT;
        AUDIO_CLK_PIN_DIRECTION = DIRECTION_OUTPUT;

        // Unlock the Peripheral Pin Select registers
        // For reference, see document DS60001250A - page 42-26

        SYSKEY = 0x0;
        SYSKEY = 0xAA996655;
        SYSKEY = 0x556699AA;
        CFGCONbits.IOLOCK = 0;

        AUDIO_MISO_PPS_REGISTER = AUDIO_MISO_PPS_VALUE;
        AUDIO_MOSI_PPS_REGISTER = AUDIO_MOSI_PPS_VALUE;
        // There is no PPS for the clock pin

        // Lock the Peripheral Pin Select registers
        CFGCONbits.IOLOCK = 1;
        SYSKEY = 0x0;

        IEC4bits.SPI3EIE = 0;
        IFS4bits.SPI3EIF = 0;
        IEC4bits.SPI3RXIE = 0;
        IFS4bits.SPI3RXIF = 0;
        IEC4bits.SPI3TXIE = 0;
        IFS4bits.SPI3TXIF = 0;

        SPI3CON  = 0x00000000;
        SPI3CON2 = 0x00000000;

        // Clear the receive buffer
        if (SPI3STATbits.SPITBE)
        {
            dummy = SPI3BUF;
        }

        // Use the hardware RX and TX FIFO in the SPI3 module
        SPI3CONbits.ENHBUF = 1;

        // Use the TX interrupt
        IFS4bits.SPI3TXIF = 0;  // Clear interrupt flag
        IPC39bits.SPI3TXIP = 4; // Interrup priority
        IEC4bits.SPI3TXIE = 1;  // Enable the interrupt

        // SPIxTXIF is set when the last transfer is shifted out of SPISR and
        // transmit operations are complete.
        SPI3CONbits.STXISEL = 0;

        // Set the baud rate
        SPI3BRG = (PBCLK_FREQ_HZ / SPI3_BAUD) / 2 - 1;

        SPI3STATbits.SPIROV = 0;

        // SPI1CON settings
        SPI3CONbits.MODE32 = 1; // Use 32 bit transmissions
        SPI3CONbits.SMP = 0;    // Input data sampled at middle of data output time
        SPI3CONbits.CKE = 0;    // Serial output data changes on transition from
                                // idle clock state to active clock state
        SPI3CONbits.CKP = 0;    // Idle state for clock is a low level

        SPI3CONbits.MSTEN = 1;  // Master mode

        SPI3CON2bits.IGNROV = 1; // Ignore Receive Overflow bit
        SPI3CON2bits.IGNTUR = 1; // Ignore Transmit Underrun bit

        SPI3CONbits.ON = 1;

        spi3_initialized = true;
    }
}

static void spi3_write(uint32_t data)
{
    spi3_write_vector(&data, 1);
}

static void spi3_write_vector(uint32_t v[], uint32_t number_of_elements)
{
    uint32_t i;
    bool tx_isr_was_enabled = IEC4bits.SPI3TXIE;

    spi3_disable_tx_interrupt();

    for (i = 0; i != number_of_elements; ++i)
    {
        if (0 != spi3_transmit_fifo.size)
        {
            spi3_transmit_fifo.last++;
        }

        if (SPI_WRITE_BUFFER_SIZE == spi3_transmit_fifo.last)
        {
            spi3_transmit_fifo.last = 0;
        }

        spi3_transmit_fifo.buffer[spi3_transmit_fifo.last] = v[i];
        spi3_transmit_fifo.size++;
    }

    if (tx_isr_was_enabled)
    {
        spi3_enable_tx_interrupt();
    }

    spi3_start_tx();
}

static void spi3_start_tx(void)
{
    spi3_disable_tx_interrupt();

    while ((!SPI3STATbits.SPITBF) && (0 != spi3_transmit_fifo.size))
    {
        SPI3BUF = spi3_transmit_fifo.buffer[spi3_transmit_fifo.first];
        spi3_transmit_fifo.size--;

        if (0 != spi3_transmit_fifo.size)
        {
            spi3_transmit_fifo.first++;

            if (SPI_WRITE_BUFFER_SIZE == spi3_transmit_fifo.first)
            {
                spi3_transmit_fifo.first = 0;
            }
        }
    }

    spi3_enable_tx_interrupt();
}

static void spi4_initialize()
{
    volatile uint32_t dummy = 0;

    if (false == spi4_initialized)
    {
        //
        // IO ports
        //
        SD_CARD_SS_OFF;
        SD_CARD_SS_PIN_DIRECTION = DIRECTION_OUTPUT;
        SD_CARD_MISO_PIN_DIRECITON = DIRECTION_INPUT;
        SD_CARD_MOSI_PIN_DIRECTION = DIRECTION_OUTPUT;
        SD_CARD_CLK_PIN_DIRECTION = DIRECTION_OUTPUT;
        SD_CARD_DETECT_PIN_DIRECTION = DIRECTION_INPUT;

        // Unlock the Peripheral Pin Select registers
        // For reference, see document DS60001250A - page 42-26

        SYSKEY = 0x0;
        SYSKEY = 0xAA996655;
        SYSKEY = 0x556699AA;
        CFGCONbits.IOLOCK = 0;

        SD_CARD_SS_PPS_REGISTER = SD_CARD_SS_PPS_VALUE;
        SD_CARD_MISO_PPS_REGISTER = SD_CARD_MISO_PPS_VALUE;
        SD_CARD_MOSI_PPS_REGISTER = SD_CARD_MOSI_PPS_VALUE;
        // There is no PPS for the clock pin

        // Lock the Peripheral Pin Select registers
        CFGCONbits.IOLOCK = 1;
        SYSKEY = 0x0;

        IEC5bits.SPI4EIE = 0;
        IFS5bits.SPI4EIF = 0;
        IEC5bits.SPI4RXIE = 0;
        IFS5bits.SPI4RXIF = 0;
        IEC5bits.SPI4TXIE = 0;
        IFS5bits.SPI4TXIF = 0;

        SPI4CON  = 0x00000000;
        SPI4CON2 = 0x00000000;

        // Clear the receive buffer
        if (SPI4STATbits.SPITBE)
        {
            dummy = SPI3BUF;
        }

        // Use the hardware RX and TX FIFO in the SPI4 module
        SPI4CONbits.ENHBUF = 1;

        // Use the TX interrupt flag as a DMA trigger
        IFS5bits.SPI4TXIF = 0;  // Clear interrupt flag

        // SPIxTXIF is set when the buffer is empty by one-half or more
        SPI4CONbits.STXISEL = 2;

        // Set the baud rate
        SPI4BRG = (PBCLK_FREQ_HZ / SPI4_BAUD) / 2 - 1;

        SPI4STATbits.SPIROV = 0;

        // SPI1CON settings
        SPI4CONbits.MODE16 = 0;     // Use 8 bit transmissions
        SPI4CONbits.MODE32 = 0;     // Use 8 bit transmissions
        SPI4CONbits.SMP = 0;        // Input data sampled at middle of data output time
        SPI4CONbits.CKE = 0;        // Serial output data changes on transition from
                                    // idle clock state to active clock state
        SPI4CONbits.CKP = 0;        // Idle state for clock is a low level

        SPI4CONbits.MSTEN = 1;      // Master mode
        //SPI4CONbits.MSSEN = 1;     // Slave select SPI support enabled

        SPI4CON2bits.IGNROV = 1;    // Ignore Receive Overflow bit
        SPI4CON2bits.IGNTUR = 1;    // Ignore Transmit Underrun bit

        SPI4CONbits.ON = 1;

        spi4_initialized = true;
    }
}

uint8_t spi4_tranceive_blocking(uint8_t data_to_send)
{
    volatile uint8_t received = 0;

    spi4_initialize();

    // Make sure that the receive FIFO is empty
    while (0 != SPI4STATbits.RXBUFELM)
    {
        received = SPI4BUF;
    }

    SPI4BUF = data_to_send;

    while (0 == SPI4STATbits.RXBUFELM)
    {
        ;   // Wait for the transaction to complete
    }

    received = SPI4BUF;

    return received;
}