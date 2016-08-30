// =============================================================================
// Include statements
// =============================================================================

#include <xc.h>

#include "gpio.h"
#include "pinmap.h"

// =============================================================================
// Private type definitions
// =============================================================================

// =============================================================================
// Global variables
// =============================================================================

// =============================================================================
// Private constants
// =============================================================================

// =============================================================================
// Private variables
// =============================================================================

// =============================================================================
// Private function declarations
// =============================================================================

// =============================================================================
// Public function definitions
// =============================================================================

void gpio_init(void)
{
    // Configure all pins as digital (not analog)
    ANSELB = 0;
    ANSELC = 0;
    ANSELD = 0;
    ANSELE = 0;
    ANSELF = 0;
    ANSELG = 0;

    //
    // LEDs
    //
    RED_LED_PIN_DIRECTION = DIRECTION_OUTPUT;
    GREEN_LED_PIN_DIRECTION = DIRECTION_OUTPUT;
    RED_LED_OFF;
    GREEN_LED_OFF;

    //
    // Buttons
    //
    LEFT_BUTTON_PIN_DIRECTION = DIRECTION_INPUT;
    UP_BUTTON_PIN_DIRECTION   = DIRECTION_INPUT;
    UP_BUTTON_PIN_DIRECTION   = DIRECTION_INPUT;
    DOWN_BUTTON_PIN_DIRECTION = DIRECTION_INPUT;

    A_BUTTON_PIN_DIRECTION    = DIRECTION_INPUT;
    B_BUTTON_PIN_DIRECTION    = DIRECTION_INPUT;

    L1_BUTTON_PIN_DIRECTION   = DIRECTION_INPUT;
    R1_BUTTON_PIN_DIRECTION   = DIRECTION_INPUT;
    
    START_BUTTON_PIN_DIRECTION  = DIRECTION_INPUT;
    SELECT_BUTTON_PIN_DIRECTION = DIRECTION_INPUT;

    //
    // UART
    //
    UART_TX_PIN = 1;
    UART_TX_PIN_DIRECTION = DIRECTION_OUTPUT;
    
    UART_RX_PIN_DIRECTION = DIRECTION_INPUT;

    //
    // SPI link to audio processor
    //
    AUDIO_SS_OFF;
    AUDIO_SS_PIN_DIRECTION = DIRECTION_OUTPUT;

    AUDIO_MISO_PIN_DIRECTION = DIRECTION_INPUT;
    AUDIO_MOSI_PIN_DIRECTION = DIRECTION_OUTPUT;

    AUDIO_CLK_PIN = 1;
    AUDIO_CLK_PIN_DIRECTION = DIRECTION_OUTPUT;

    //
    // SD card SPI interface
    //
    SD_CARD_SS_OFF;
    SD_CARD_SS_PIN_DIRECTION = DIRECTION_OUTPUT;

    SD_CARD_MISO_PIN_DIRECITON = DIRECTION_INPUT;
    SD_CARD_MOSI_PIN_DIRECTION = DIRECTION_OUTPUT;

    SD_CARD_CLK_PIN = 0;
    SD_CARD_CLK_PIN_DIRECTION = DIRECTION_OUTPUT;

    SD_CARD_DETECT_PIN_DIRECTION = DIRECTION_INPUT;

    //
    // LCD interface
    //
    LCD_SS_OFF;
    LCD_SS_PIN_DIRECTION = DIRECTION_OUTPUT;

    LCD_MOSI_PIN_DIRECTION = DIRECTION_OUTPUT;

    LCD_CLK_PIN = 1;
    LCD_CLK_PIN_DIRECTION = DIRECTION_OUTPUT;

    LCD_RESET_OFF;
    LCD_RESET_PIN_DIRECTION = DIRECTION_OUTPUT;

    //
    // GPU interface
    //
    GPU_SS_OFF;
    GPU_SS_PIN_DIRECTION = DIRECTION_OUTPUT;

    GPU_MISO_PIN_DIRECTION = DIRECTION_INPUT;
    GPU_MOSI_PIN_DIRECTION = DIRECTION_OUTPUT;

    GPU_CLK_PIN = 0;
    GPU_CLK_PIN_DIRECTION = DIRECTION_OUTPUT;

    GPU_GPIO_PIN_DIRECTIOIN = DIRECTION_INPUT;
    GPU_INT_PIN_DIRECTION  = DIRECTION_INPUT;

    GPU_POWER_DOWN;
    GPU_POWER_DOWN_PIN_DIRECTION = DIRECTION_OUTPUT;
    
    //
    // Test points
    //
    TP7_PIN_DIRECTION = DIRECTION_OUTPUT;
    TP8_PIN_DIRECTION = DIRECTION_OUTPUT;
    TP9_PIN_DIRECTION = DIRECTION_OUTPUT;
    TP10_PIN_DIRECTION = DIRECTION_OUTPUT;

    TP7_PIN = 0;
    TP8_PIN = 0;
    TP9_PIN = 0;
    TP10_PIN = 0;

    //
    // Unconnected pins
    //
    NC0_PIN_DIRECTION = DIRECTION_OUTPUT;
    NC1_PIN_DIRECTION = DIRECTION_OUTPUT;
    NC2_PIN_DIRECTION = DIRECTION_OUTPUT;
    NC3_PIN_DIRECTION = DIRECTION_OUTPUT;
    NC4_PIN_DIRECTION = DIRECTION_OUTPUT;
    NC5_PIN_DIRECTION = DIRECTION_OUTPUT;

    NC0_PIN = 0;
    NC1_PIN = 0;
    NC2_PIN = 0;
    NC3_PIN = 0;
    NC4_PIN = 0;
    NC5_PIN = 0;
}

// =============================================================================
// Private function definitions
// =============================================================================
