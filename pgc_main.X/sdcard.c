/*
 *
 * References:
 * Fat file system:
 * https://github.com/thenickdude/asyncfatfs
 *
 * SD card:
 * Physical Layer Simplified Specification,
 * https://www.sdcard.org/downloads/pls/
 */

// =============================================================================
// Include statements
// =============================================================================
#include <stdint.h>
#include <stdbool.h>

#include "sdcard.h"
#include "spi.h"
#include "uart.h"
#include "debug_util.h"
#include "pinmap.h"
#include "wait_timer.h"

// =============================================================================
// Private type definitions
// =============================================================================

typedef union
{
    struct
    {
        unsigned in_idle_state:1;
        unsigned erase_reset:1;
        unsigned illegal_command:1;
        unsigned com_crc_error:1;
        unsigned erase_sequence_error:1;
        unsigned address_error:1;
        unsigned parameter_error:1;
    };
    uint8_t r1;
} r1_response_t;

typedef union
{
    struct
    {
        unsigned card_is_locked:1;
        unsigned wp_erase_skip:1;
        unsigned error:1;
        unsigned card_ecc_failed:1;
        unsigned wp_violation:1;
        unsigned erase_param:1;
        unsigned out_of_range:1;
        unsigned in_idle_state:1;
        unsigned erase_reset:1;
        unsigned illegal_command:1;
        unsigned com_crc_error:1;
        unsigned erase_sequence_error:1;
        unsigned address_error:1;
        unsigned parameter_error:1;
    };
    uint16_t r2;
} r2_response_t;

typedef union
{
    struct
    {
        unsigned in_idle_state:1;
        unsigned erase_reset:1;
        unsigned illegal_command:1;
        unsigned com_crc_error:1;
        unsigned erase_sequence_error:1;
        unsigned address_error:1;
        unsigned parameter_error:1;
        unsigned cmd_ver:4;
        unsigned reserved:16;
        unsigned voltage_ok:4;
        unsigned echo_back:8;
    };
    struct
    {
        unsigned byte_1:8;
        unsigned byte_2:8;
        unsigned byte_3:8;
        unsigned byte_4:8;
        unsigned byte_5:8;
    };
} r7_response_t;

typedef union
{
    r1_response_t r1;
    r2_response_t r2;
    r7_response_t r7;
} response_t;

typedef enum response_type_t
{
    NO_RESPONSE,
    R1,
    R1b,
    R3,
    R7
} response_type_t;

// =============================================================================
// Global variables
// =============================================================================

// =============================================================================
// Private constants
// =============================================================================

//
// SD commands
// Reference: sdcard.org, SD Specifications, Part 1, Physical Layer
//
typedef enum command_t
{
    CMD_GO_IDLE_STATE          = 0,
    CMD_SEND_OP_COND           = 1,
    CMD_SWITCH_FUNC            = 6,
    CMD_SEND_IF_COND           = 8,
    CMD_SEND_CSD               = 9,
    CMD_SEND_CID               = 10,
    CMD_STOP_TRANSMISSION      = 12,
    CMD_SET_BLOCKLEN           = 16,
    CMD_READ_SINGLE_BLOCK      = 17,
    CMD_READ_MULTIPLE_BLOCK    = 18,
    CMD_WRITE_BLOCK            = 24,
    CMD_WRITE_MULTIPLE_BLOCK   = 25,
    CMD_PROGRAM_CSD            = 27,
    CMD_SET_WRITE_PROT         = 28,
    CMD_CLR_WRITE_PROT         = 29,
    CMD_SEND_WRITE_PROT        = 30,
    CMD_ERASE_WR_BLK_START_ADDR = 32,
    CMD_ERASE_WR_BLK_END_ADDR  = 33,
    CMD_LOCK_UNLOCK            = 42,
    CMD_APP_CMD                = 55,
    CMD_GEN_CMD                = 56,
    CMD_READ_OCR               = 58,
    CMD_CRC_ON_OFF             = 59,
    ACMD_SD_STATUS             = 13,
    ACMD_SD_SEND_OP_COND       = 41
} command_t;

// =============================================================================
// Private variables
// =============================================================================

// =============================================================================
// Private function declarations
// =============================================================================

/**
 * @brief Sends a command to the SD card.
 * @details This is a blocking operation.
 * @param cmd_number - The command number.
 * @param arg - The command argument.
 * @param response - Where to store the response from the SD card.
 * @return The type of response placed in the response parameter.
 */
static response_type_t send_command_blocking(uint8_t cmd_number,
                                             uint32_t arg,
                                             response_t* response);
// =============================================================================
// Public function definitions
// =============================================================================

/*
 * Initialization sequence:
 * 0.   Wait for at least 1ms
 * 1.   11x8 clock pulses with CS high
 * 2.   Send CMD0
 * 3.   Send CMD8
 * 4.   Send ACMD41 until in idle state
 */
bool sdcard_init(void)
{
    uint32_t i;
    response_t response;
    memset(&response, 0xFF, sizeof(response_t));

    // Power up delay for the SD card
    wait_timer_ms(1);

    spi_init(SPI_DEVICE_SDCARD);

    for (i = 0; i != 10; ++i)
    {
        SD_CARD_SS_OFF;
        spi_byte_tranceive_blocking(SPI_DEVICE_SDCARD, 0xFF);
    }

    (void)send_command_blocking(CMD_GO_IDLE_STATE, 0,&response);

    (void)send_command_blocking(CMD_SEND_IF_COND, 0x000001AA, &response);


    i = 0;
    while (response.r1.in_idle_state && (50 != i++))
    {
        memset(&response, 0xFF, sizeof(response_t));
        (void)send_command_blocking(CMD_APP_CMD, 0, &response);

        memset(&response, 0xFF, sizeof(response_t));
        (void)send_command_blocking(ACMD_SD_SEND_OP_COND,
                                    0x40000000, // Supports SDHC and SDXC
                                    &response);
        wait_timer_us(1000);
    }

    if (response.r1.in_idle_state)
    {
        // No SD card of v2.0 or later found.
        sprintf(g_debug_util_char_buffer,
                "%s - SD card could not enter idle state%s",
                ERROR_TAG, NEWLINE);
        uart_write_string(g_debug_util_char_buffer);
    }

    return (0 != response.r1.in_idle_state);
}

/**
 * Read the 512-byte block with the given index into the given 512-byte buffer.
 *
 * When the read completes, your callback will be called. If the read was successful, the buffer pointer will be the
 * same buffer you originally passed in, otherwise the buffer will be set to NULL.
 *
 * You must keep the pointer to the buffer valid until the operation completes!
 *
 * Returns:
 *     true - The operation was successfully queued for later completion, your callback will be called later
 *     false - The operation could not be started due to the card being busy (try again later).
 */
bool sdcard_readBlock(uint32_t blockIndex,
                      uint8_t *buffer,
                      sdcard_operationCompleteCallback_c callback,
                      uint32_t callbackData)
{
    ;// TODO implement error_occurredsdcard_readBlock
    bool error = false;
    r7_response_t r7_response;
    memset(&r7_response, 0xFF, sizeof(r7_response));
    
    //send_command(CMD_READ_SINGLE_BLOCK, blockIndex, &r7_response);
    error |= (0 != r7_response.byte_1);
}

/**
 * Write the 512-byte block from the given buffer into the block with the given index.
 *
 * If the write does not complete immediately, your callback will be called later. If the write was successful, the
 * buffer pointer will be the same buffer you originally passed in, otherwise the buffer will be set to NULL.
 *
 * Returns:
 *     SDCARD_OPERATION_IN_PROGRESS - Your buffer is currently being transmitted to the card and your callback will be
 *                                    called later to report the completion. The buffer pointer must remain valid until
 *                                    that time.
 *     SDCARD_OPERATION_SUCCESS     - Your buffer has been transmitted to the card now.
 *     SDCARD_OPERATION_BUSY        - The card is already busy and cannot accept your write
 *     SDCARD_OPERATION_FAILURE     - Your write was rejected by the card, card will be reset
 */
sdcardOperationStatus_e sdcard_writeBlock(uint32_t blockIndex,
                                          uint8_t *buffer,
                                          sdcard_operationCompleteCallback_c callback,
                                          uint32_t callbackData)
{
    ;// TODO implement sdcard_writeBlock
}

/**
 * Call periodically for the SD card to perform in-progress transfers.
 *
 * Returns true if the card is ready to accept commands.
 */
bool sdcard_poll()
{
    ; // TODO impement sdcard_poll
}

/**
 * Begin writing a series of consecutive blocks beginning at the given block index. This will allow (but not require)
 * the SD card to pre-erase the number of blocks you specifiy, which can allow the writes to complete faster.
 *
 * Afterwards, just call sdcard_writeBlock() as normal to write those blocks consecutively.
 *
 * The multi-block write will be aborted automatically when writing to a non-consecutive address, or by performing a
 * read. You can abort it manually by calling sdcard_endWriteBlocks().
 *
 * Returns:
 *     SDCARD_OPERATION_SUCCESS     - Multi-block write has been queued
 *     SDCARD_OPERATION_BUSY        - The card is already busy and cannot accept your write
 *     SDCARD_OPERATION_FAILURE     - A fatal error occured, card will be reset
 */
sdcardOperationStatus_e sdcard_beginWriteBlocks(uint32_t blockIndex,
                                                uint32_t blockCount)
{
    ; // TODO implement sdcard_beginWriteBlocks
}

/**
 * Abort a multiple-block write early (before all the `blockCount` blocks had been written).
 *
 * Returns:
 *     SDCARD_OPERATION_SUCCESS     - Multi-block write has been cancelled, or no multi-block write was in progress.
 *     SDCARD_OPERATION_BUSY        - The card is busy with another operation and could not cancel the multi-block write.
 */
sdcardOperationStatus_e sdcard_endWriteBlocks()
{
    ;// TODO implement sdcard_endWriteBlocks
}

void sdcard_setProfilerCallback(sdcard_profilerCallback_c callback)
{
    ; // TODO implement sdcard_setProfilerCallback
}

// =============================================================================
// Private function definitions
// =============================================================================

static response_type_t send_command_blocking(uint8_t cmd_number,
                                             uint32_t arg,
                                             response_t* response)
{
    // Start bit is active low
    const uint8_t START_BIT_MASK = 0x80;
    // Transmission bit is active high
    const uint8_t TRANSMISSION_BIT_MASK = 0x40;
    const uint8_t NBR_OF_BYTES = 6;
    const int RESPONSE_TIMEOUT = 10;

    int i;
    bool response_found;
    uint8_t first_byte_in_response;
    response_type_t response_type_returned = NO_RESPONSE;

    SD_CARD_SS_ON;

    uint8_t data_to_send[NBR_OF_BYTES];
    data_to_send[0] = (TRANSMISSION_BIT_MASK | cmd_number) & ~START_BIT_MASK;
    data_to_send[1] = arg >> (3 * 8);
    data_to_send[2] = arg >> (2 * 8);
    data_to_send[3] = arg >> (1 * 8);
    data_to_send[4] = arg;
    // CRC, only used for CMD0 and CMD8, so hard code it.
    if (CMD_GO_IDLE_STATE == cmd_number)
    {
        data_to_send[5] = 0x95;
    }
    else
    {
        // CMD 8
        data_to_send[5] = 0x87;
    }

    for (i = 0; i != NBR_OF_BYTES; ++i)
    {
        spi_byte_tranceive_blocking(SPI_DEVICE_SDCARD, data_to_send[i]);
    }

    // Wait for the response
    i = 0;
    response_found = false;
    while ((false == response_found) && (++i != RESPONSE_TIMEOUT))
    {
        first_byte_in_response = spi_byte_tranceive_blocking(SPI_DEVICE_SDCARD,
                                                             0xFF);
        response_found = (0 == (first_byte_in_response & START_BIT_MASK));
    }

    if (i == RESPONSE_TIMEOUT)
    {
        sprintf(g_debug_util_char_buffer, "%s - SD card response timeout%s",
                ERROR_TAG, NEWLINE);
        uart_write_string(g_debug_util_char_buffer);
        SD_CARD_SS_OFF;
        return NO_RESPONSE;
    }

    switch (cmd_number)
    {
    case CMD_GO_IDLE_STATE:
    case CMD_SEND_OP_COND:
    case CMD_SWITCH_FUNC:
    case CMD_SEND_CSD:
    case CMD_SEND_CID:
    case CMD_SET_BLOCKLEN:
    case CMD_READ_SINGLE_BLOCK:
    case CMD_READ_MULTIPLE_BLOCK:
    case CMD_WRITE_BLOCK:
    case CMD_WRITE_MULTIPLE_BLOCK:
    case CMD_PROGRAM_CSD:
    case CMD_SEND_WRITE_PROT:
    case CMD_ERASE_WR_BLK_START_ADDR:
    case CMD_ERASE_WR_BLK_END_ADDR:
    case CMD_LOCK_UNLOCK:
    case CMD_APP_CMD:
    case CMD_GEN_CMD:
    case CMD_CRC_ON_OFF:
    case ACMD_SD_SEND_OP_COND:
        response_type_returned = R1;
        response->r1.r1 = first_byte_in_response;
        break;

    case CMD_SEND_IF_COND:
        response_type_returned = R7;
        response->r7.byte_1 = first_byte_in_response;
        response->r7.byte_2 = spi_byte_tranceive_blocking(SPI_DEVICE_SDCARD,
                                                          0xFF);
        response->r7.byte_3 = spi_byte_tranceive_blocking(SPI_DEVICE_SDCARD,
                                                          0xFF);
        response->r7.byte_4 = spi_byte_tranceive_blocking(SPI_DEVICE_SDCARD,
                                                          0xFF);
        response->r7.byte_5 = spi_byte_tranceive_blocking(SPI_DEVICE_SDCARD,
                                                          0xFF);
        break;

    default:
        response_type_returned = NO_RESPONSE;
        break;
    }

    SD_CARD_SS_OFF;
}