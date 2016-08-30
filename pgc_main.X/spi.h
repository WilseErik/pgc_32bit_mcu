#ifndef SPI_H
#define	SPI_H

#ifdef	__cplusplus
//extern "C" {
#endif

// =============================================================================
// Include statements
// =============================================================================

// =============================================================================
// Public type definitions
// =============================================================================

typedef enum spi_device_t
{
    SPI_DEVICE_FT801,
    SPI_DEVICE_SDCARD,
    SPI_DEVICE_DSP
} spi_device_t;

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
 * @brief Initializes the SPI interface to one external device.
 * @param spi_device - the spi interface to initialize.
 * @return void
 */
void spi_init(spi_device_t spi_device);

/**
 * @brief Sends and receives a byte of data.
 * @param data_to_send - The byte to send over the spi interface.
 * @return The returned byte from the spi interface.
 */
uint8_t spi_byte_tranceive_blocking(spi_device_t spi_device,
                                    uint8_t data_to_send);

/**
 * @brief Sends a 32 bit double word over the spi interface.
 * @details This is an asynchronous operation.
 * @param spi_device - the SPI interface to use.
 * @param data - the 32 bit double word to write.
 */
void spi_write_dword(spi_device_t spi_device, uint32_t data);

/**
 * @brief Sends a series of 32 bit dwords over a spi interface.
 * @param spi_device - the spi interface to use.
 * @param data - the data to send.
 * @param number_of_elements - the number of uint32_t elements in data.
 */
void spi_write_dword_vect(spi_device_t spi_device,
                          uint32_t data[],
                          uint32_t number_of_elements);

/**
 * @brief Updates the baud rate of the spi bus for the SD card.
 * @param new_baud - the baud rate to change to.
 */
void spi_update_sd_card_baud(uint32_t new_baud);

/**
 * @brief Prints debug information regardning the spi module over the uart.
 * @param spi_device - The spi interface to use.
 */
void spi_print_debug_status(spi_device_t spi_device);

#ifdef	__cplusplus
}
#endif

#endif	/* SPI_H */

