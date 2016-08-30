/* 
 * File:   pinmap.h
 * Author: Erik
 *
 * Created on den 2 februari 2016, 20:47
 */

#ifndef PINMAP_H
#define	PINMAP_H

#ifdef	__cplusplus
//extern "C" {
#endif

// =============================================================================
// Include statements
// =============================================================================
#include <xc.h>

// =============================================================================
// Public type definitions
// =============================================================================


/*********************************************************
 * Peripheral Pin Selection (PPS) - Output pin selection *
 ********************************************************/

/* Use pps_output_source1_t for the following RPn Port Pins:
 *      RPD2
 *      PRG8
 *      RPD10
 *      RPF1
 *      RPB9
 *      RPB10
 *      RPC14
 *      RPB5
 *      RPC1
 *      RPD14
 *      RPG1
 *      RPA14
 *      RPD6
 * Reference: DS60001191B-page 239
 */
typedef enum pps_output_source1_t
{
    PPS_OUT_SRC1_NULL       = 0x0,
    PPS_OUT_SRC1_U3TX       = 0x1,
    PPS_OUT_SRC1_U4RTS      = 0x2,
    PPS_OUT_SRC1_RESERVED1  = 0x3,
    PPS_OUT_SRC1_RESERVED2  = 0x4,
    PPS_OUT_SRC1_SDO1       = 0x5,
    PPS_OUT_SRC1_SDO2       = 0x6,
    PPS_OUT_SRC1_SDO3       = 0x7,
    PPS_OUT_SRC1_RESERVED3  = 0x8,
    PPS_OUT_SRC1_SDO5       = 0x9,
    PPS_OUT_SRC1_SS6        = 0xA,
    PPS_OUT_SRC1_OC3        = 0xB,
    PPS_OUT_SRC1_OC6        = 0xC,
    PPS_OUT_SRC1_REFCLKO4   = 0xD,
    PPS_OUT_SRC1_C2OUT      = 0xE,
    PPS_OUT_SRC1_C1TX       = 0xF
} pps_output_source1_t;

/* Use pps_output_source2_t for the following RPn Port Pins:
 *      RPD3
 *      PRG7
 *      RPD11
 *      RPF0
 *      RPB1
 *      RPE5
 *      RPC13
 *      RPB3
 *      RPC4
 *      RPD15
 *      RPG0
 *      RPA15
 *      RPD7
 * Reference: DS60001191B-page 239
 */
typedef enum pps_output_source2_t
{
    PPS_OUT_SRC2_NULL       = 0x0,
    PPS_OUT_SRC2_U1TX       = 0x1,
    PPS_OUT_SRC2_U2RTS      = 0x2,
    PPS_OUT_SRC2_U5TX       = 0x3,
    PPS_OUT_SRC2_U6RTS      = 0x4,
    PPS_OUT_SRC2_SDO1       = 0x5,
    PPS_OUT_SRC2_SDO2       = 0x6,
    PPS_OUT_SRC2_SDO3       = 0x7,
    PPS_OUT_SRC2_SD04       = 0x8,
    PPS_OUT_SRC2_SDO5       = 0x9,
    PPS_OUT_SRC2_RESERVED1  = 0xA,
    PPS_OUT_SRC2_OC4        = 0xB,
    PPS_OUT_SRC2_OC7        = 0xC,
    PPS_OUT_SRC2_RESERVED2  = 0xD,
    PPS_OUT_SRC2_RESERVED3  = 0xE,
    PPS_OUT_SRC2_REFCLKO1   = 0xF
} pps_output_source2_t;

/* Use pps_output_source3_t for the following RPn Port Pins:
 *      RPD9
 *      PRG6
 *      RPB8
 *      RPB15
 *      RPD4
 *      RPB0
 *      RPE3
 *      RPB7
 *      RPF12
 *      RPD12
 *      RPF8
 *      RPC3
 *      RPE9
 * Reference: DS60001191B-page 239
 */
typedef enum pps_output_source3_t
{
    PPS_OUT_SRC3_NULL       = 0x0,
    PPS_OUT_SRC3_U3RTS      = 0x1,
    PPS_OUT_SRC3_U4TX       = 0x2,
    PPS_OUT_SRC3_RESERVED1  = 0x3,
    PPS_OUT_SRC3_U6TX       = 0x4,
    PPS_OUT_SRC3_SS1        = 0x5,
    PPS_OUT_SRC3_RESERVED2  = 0x6,
    PPS_OUT_SRC3_SS3        = 0x7,
    PPS_OUT_SRC3_SS4        = 0x8,
    PPS_OUT_SRC3_SS5        = 0x9,
    PPS_OUT_SRC3_SDO6       = 0xA,
    PPS_OUT_SRC3_OC5        = 0xB,
    PPS_OUT_SRC3_OC8        = 0xC,
    PPS_OUT_SRC3_RESERVED3  = 0xD,
    PPS_OUT_SRC3_C1OUT      = 0xE,
    PPS_OUT_SRC3_REFCLKO3   = 0xF
} pps_output_source3_t;

/* Use pps_output_source4_t for the following RPn Port Pins:
 *      RPD1
 *      PRG9
 *      RPB14
 *      RPD0
 *      RPB6
 *      RPD5
 *      RPB2
 *      RPF3
 *      RPF13
 *      RPC2
 *      RPE8
 *      RPF2
 * Reference: DS60001191B-page 240
 */
typedef enum pps_output_source4_t
{
    PPS_OUT_SRC4_NULL       = 0x0,
    PPS_OUT_SRC4_U1RTS      = 0x1,
    PPS_OUT_SRC4_U2TX       = 0x2,
    PPS_OUT_SRC4_U5RTS      = 0x3,
    PPS_OUT_SRC4_U6TX       = 0x4,
    PPS_OUT_SRC4_RESERVED1  = 0x5,
    PPS_OUT_SRC4_SS2        = 0x6,
    PPS_OUT_SRC4_RESERVED2  = 0x7,
    PPS_OUT_SRC4_SDO4       = 0x8,
    PPS_OUT_SRC4_RESERVED3  = 0x9,
    PPS_OUT_SRC4_SDO6       = 0xA,
    PPS_OUT_SRC4_OC2        = 0xB,
    PPS_OUT_SRC4_OC1        = 0xC,
    PPS_OUT_SRC4_OC9        = 0xD,
    PPS_OUT_SRC4_RESERVED4  = 0xE,
    PPS_OUT_SRC4_CT2TX      = 0xF
} pps_output_source4_t;



/********************************************************
 * Peripheral Pin Selection (PPS) - Input pin selection *
 *******************************************************/

/* Use pps_input_source1_t for the following peripheral pins:
 *      INT3
 *      T2CK
 *      T6CK
 *      IC3
 *      IC7
 *      U1RX
 *      U2CTS
 *      U5RX
 *      U6CTS
 *      SDI1
 *      SDI3
 *      SDI5
 *      SS6
 *      REFCLKI1
 * Reference: DS60001191B-page 236
 */
typedef enum pps_input_source1_t
{
    PPS_IN_SRC1_RPD2        = 0x0,
    PPS_IN_SRC1_RPG8        = 0x1,
    PPS_IN_SRC1_RPF4        = 0x2,
    PPS_IN_SRC1_RPD10       = 0x3,
    PPS_IN_SRC1_RPF1        = 0x4,
    PPS_IN_SRC1_RPB9        = 0x5,
    PPS_IN_SRC1_RPB10       = 0x6,
    PPS_IN_SRC1_RPC14       = 0x7,
    PPS_IN_SRC1_RPB5        = 0x8,
    PPS_IN_SRC1_RESERVED1   = 0x9,
    PPS_IN_SRC1_RPC1        = 0xA,
    PPS_IN_SRC1_RPD14       = 0xB,
    PPS_IN_SRC1_RPG1        = 0xC,
    PPS_IN_SRC1_RPA14       = 0xD,
    PPS_IN_SRC1_RPD6        = 0xE,
    PPS_IN_SRC1_RESERVED2   = 0xF
} pps_input_source1_t;

/* Use pps_input_source2_t for the following peripheral pins:
 *      INT4
 *      T5CK
 *      T7CK
 *      IC4
 *      IC8
 *      U3RX
 *      U4CTS
 *      SDI2
 *      SDI4
 *      C1RX
 *      REFCLKI14
 * Reference: DS60001191B-page 236
 */
typedef enum pps_input_source2_t
{
    PPS_IN_SRC2_RPD3        = 0x0,
    PPS_IN_SRC2_RPG7        = 0x1,
    PPS_IN_SRC2_RPF5        = 0x2,
    PPS_IN_SRC2_RPD11       = 0x3,
    PPS_IN_SRC2_RPF0        = 0x4,
    PPS_IN_SRC2_RPB1        = 0x5,
    PPS_IN_SRC2_RPE5        = 0x6,
    PPS_IN_SRC2_RPC13       = 0x7,
    PPS_IN_SRC2_RPB3        = 0x8,
    PPS_IN_SRC2_RESERVED1   = 0x9,
    PPS_IN_SRC2_RPC4        = 0xA,
    PPS_IN_SRC2_RPD15       = 0xB,
    PPS_IN_SRC2_RPG0        = 0xC,
    PPS_IN_SRC2_RPA15       = 0xD,
    PPS_IN_SRC2_RPD7        = 0xE,
    PPS_IN_SRC2_RESERVED2   = 0xF
} pps_input_source2_t;

/* Use pps_input_source3_t for the following peripheral pins:
 *      INT2
 *      T3CK
 *      T8CK
 *      IC2
 *      IC5
 *      IC9
 *      U1CTS
 *      U2RX
 *      U5CTS
 *      SS1
 *      SS3
 *      SS4
 *      SS5
 *      C2RX
 * Reference: DS60001191B-page 236
 */
typedef enum pps_input_source3_t
{
    PPS_IN_SRC3_RPD9        = 0x0,
    PPS_IN_SRC3_RPG6        = 0x1,
    PPS_IN_SRC3_RPB8        = 0x2,
    PPS_IN_SRC3_RPB15       = 0x3,
    PPS_IN_SRC3_RPD4        = 0x4,
    PPS_IN_SRC3_RPB0        = 0x5,
    PPS_IN_SRC3_RPE3        = 0x6,
    PPS_IN_SRC3_RPB7        = 0x7,
    PPS_IN_SRC3_RESERVED1   = 0x8,
    PPS_IN_SRC3_RPF12       = 0x9,
    PPS_IN_SRC3_RPD12       = 0xA,
    PPS_IN_SRC3_RPF8        = 0xB,
    PPS_IN_SRC3_RPC3        = 0xC,
    PPS_IN_SRC3_RPE9        = 0xD,
    PPS_IN_SRC3_RESERVED2   = 0xE,
    PPS_IN_SRC3_RESERVED3   = 0xF
} pps_input_source3_t;

/* Use pps_input_source4_t for the following peripheral pins:
 *      INT1
 *      T4CK
 *      T9CK
 *      IC1
 *      IC6
 *      U3CTS
 *      U4RX
 *      U6RX
 *      SS2
 *      SDI6
 *      OCFA
 *      REFCLKI3
 * Reference: DS60001191B-page 236
 */
typedef enum pps_input_source4_t
{
    PPS_IN_SRC4_RPD1        = 0x0,
    PPS_IN_SRC4_RPG9        = 0x1,
    PPS_IN_SRC4_RPB14       = 0x2,
    PPS_IN_SRC4_RPD0        = 0x3,
    PPS_IN_SRC4_RESERVED1   = 0x4,
    PPS_IN_SRC4_RPB6        = 0x5,
    PPS_IN_SRC4_RPD5        = 0x6,
    PPS_IN_SRC4_RPB2        = 0x7,
    PPS_IN_SRC4_RPF3        = 0x8,
    PPS_IN_SRC4_RPF13       = 0x9,
    PPS_IN_SRC4_NO_CONNECT  = 0xA,
    PPS_IN_SRC4_RPF2        = 0xB,
    PPS_IN_SRC4_RPC2        = 0xC,
    PPS_IN_SRC4_RPE8        = 0xD,
    PPS_IN_SRC4_RESERVED2   = 0xE,
    PPS_IN_SRC4_RESERVED3   = 0xF
} pps_input_source4_t;


// =============================================================================
// Global constatants
// =============================================================================

#define DIRECTION_INPUT     (1)
#define DIRECTION_OUTPUT    (0)

//
//  LEDs
//
#define RED_LED_PIN                 LATBbits.LATB15
#define RED_LED_PIN_DIRECTION       TRISBbits.TRISB15
#define RED_LED_ON                  (RED_LED_PIN = 1)
#define RED_LED_OFF                 (RED_LED_PIN = 0)
#define RED_LED_TOGGLE              (RED_LED_PIN = !RED_LED_PIN)

#define GREEN_LED_PIN               LATCbits.LATC12
#define GREEN_LED_PIN_DIRECTION     TRISCbits.TRISC12
#define GREEN_LED_ON                (GREEN_LED_PIN = 1)
#define GREEN_LED_OFF               (GREEN_LED_PIN = 0)
#define GREEN_LED_TOGGLE            (GREEN_LED_PIN = !GREEN_LED_PIN)


//
// Buttons
//
#define LEFT_BUTTON_PIN             PORTEbits.E0
#define LEFT_BUTTON_PIN_DIRECTION   TRISEbits.TRISE0

#define UP_BUTTON_PIN               PORTEbits.E1
#define UP_BUTTON_PIN_DIRECTION     TRISEbits.TRISE1

#define RIGHT_BUTTON_PIN            PORTEbits.E2
#define RIGHT_BUTTON_PIN_DIRECTION  TRISEbits.TRISE2

#define DOWN_BUTTON_PIN             PORTEbits.E3
#define DOWN_BUTTON_PIN_DIRECTION   TRISEbits.TRISE3

#define A_BUTTON_PIN                PORTEbits.E4
#define A_BUTTON_PIN_DIRECTION      TRISEbits.TRISE4

#define B_BUTTON_PIN                PORTEbits.RE5
#define B_BUTTON_PIN_DIRECTION      TRISEbits.TRISE5

#define L1_BUTTON_PIN               PORTEbits.RE7
#define L1_BUTTON_PIN_DIRECTION     TRISEbits.TRISE7

#define R1_BUTTON_PIN               PORTDbits.RD3
#define R1_BUTTON_PIN_DIRECTION     TRISDbits.TRISD3

#define START_BUTTON_PIN            PORTCbits.C15
#define START_BUTTON_PIN_DIRECTION  TRISCbits.TRISC15

#define SELECT_BUTTON_PIN           PORTCbits.C14
#define SELECT_BUTTON_PIN_DIRECTION TRISCbits.TRISC14


//
// UART
//
#define UART_TX_PIN                 LATBbits.LATB3
#define UART_TX_PIN_DIRECTION       TRISBbits.TRISB3
#define UART_TX_PPS_REGISTER        RPB3Rbits.RPB3R
#define UART_TX_PPS_VALUE           PPS_OUT_SRC2_U1TX

#define UART_RX_PIN                 PORTBbits.B5
#define UART_RX_PIN_DIRECTION       TRISBbits.TRISB5
#define UART_RX_PPS_REGISTER        U1RXR
#define UART_RX_PPS_VALUE           PPS_IN_SRC1_RPB5


//
// SPI link to audio processor
//
#define AUDIO_SS_PIN                LATBbits.LATB8
#define AUDIO_SS_PIN_DIRECTION      TRISBbits.TRISB8
#define AUDIO_SS_ON                 (AUDIO_SS_PIN = 0)
#define AUDIO_SS_OFF                (AUDIO_SS_PIN = 1)

#define AUDIO_MISO_PIN              PORTBbits.B9
#define AUDIO_MISO_PIN_DIRECTION    TRISBbits.TRISB9
#define AUDIO_MISO_PPS_REGISTER     SDI3R
#define AUDIO_MISO_PPS_VALUE        PPS_IN_SRC1_RPB9

#define AUDIO_MOSI_PIN              LATBbits.LATB10
#define AUDIO_MOSI_PIN_DIRECTION    TRISBbits.TRISB10
#define AUDIO_MOSI_PPS_REGISTER     RPB10R
#define AUDIO_MOSI_PPS_VALUE        PPS_OUT_SRC1_SDO3

#define AUDIO_CLK_PIN               LATBbits.LATB14
#define AUDIO_CLK_PIN_DIRECTION     TRISBbits.TRISB14
#define AUDIO_CLK_PPS_REGISTER      RPB14R
// There is no PPS for the CLK pin

//
// SD card SPI interface
//
#define SD_CARD_SS_PIN              LATDbits.LATD9
#define SD_CARD_SS_PIN_DIRECTION    TRISDbits.TRISD9
#define SD_CARD_SS_ON               (SD_CARD_SS_PIN = 0)
#define SD_CARD_SS_OFF              (SD_CARD_SS_PIN = 1)
#define SD_CARD_SS_PPS_REGISTER     RPD9R
#define SD_CARD_SS_PPS_VALUE        PPS_OUT_SRC3_SS4

#define SD_CARD_MISO_PIN            PORTDbits.RD11
#define SD_CARD_MISO_PIN_DIRECITON  TRISDbits.TRISD11
#define SD_CARD_MISO_PPS_REGISTER   SDI4R
#define SD_CARD_MISO_PPS_VALUE      PPS_IN_SRC2_RPD11

#define SD_CARD_MOSI_PIN            LATCbits.LATC13
#define SD_CARD_MOSI_PIN_DIRECTION  TRISCbits.TRISC13
#define SD_CARD_MOSI_PPS_REGISTER   RPC13R
#define SD_CARD_MOSI_PPS_VALUE      PPS_OUT_SRC2_SD04

#define SD_CARD_CLK_PIN             LATDbits.LATD10
#define SD_CARD_CLK_PIN_DIRECTION   TRISDbits.TRISD10
#define SD_CARD_CLK_PPS_REGISTER    RPD10R
// There is no PPS for the CLK pin

#define SD_CARD_DETECT_PIN           PORTDbits.RD5
#define SD_CARD_DETECT_PIN_DIRECTION TRISDbits.TRISD5

//
// LCD interface
//
#define LCD_SS_PIN                  LATDbits.LATD4
#define LCD_SS_PIN_DIRECTION        TRISDbits.TRISD4
#define LCD_SS_ON                   (LCD_SS_PIN = 0)
#define LCD_SS_OFF                  (LCD_SS_PIN = 1)

// No MISO pin

#define LCD_MOSI_PIN                LATDbits.LATD2
#define LCD_MOSI_PIN_DIRECTION      TRISDbits.TRISD2

#define LCD_CLK_PIN                 LATDbits.LATD1
#define LCD_CLK_PIN_DIRECTION       TRISDbits.TRISD1

#define LCD_RESET_PIN               LATDbits.LATD0
#define LCD_RESET_PIN_DIRECTION     TRISDbits.TRISD0
#define LCD_RESET_ON                (LCD_RESET_PIN = 0)
#define LCD_RESET_OFF               (LCD_RESET_PIN = 1)

//
// GPU interface
//
#define GPU_SS_PIN                  LATGbits.LATG9
#define GPU_SS_PIN_DIRECTION        TRISGbits.TRISG9
#define GPU_SS_ON                   (GPU_SS_PIN = 0)
#define GPU_SS_OFF                  (GPU_SS_PIN = 1)

#define GPU_MISO_PIN                PORTGbits.G7
#define GPU_MISO_PIN_DIRECTION      TRISGbits.TRISG7

#define GPU_MOSI_PIN                LATGbits.LATG8
#define GPU_MOSI_PIN_DIRECTION      TRISGbits.TRISG8

#define GPU_CLK_PIN                 LATGbits.LATG6
#define GPU_CLK_PIN_DIRECTION       TRISGbits.TRISG6

#define GPU_GPIO_PIN                PORTFbits.F0
#define GPU_GPIO_PIN_DIRECTIOIN     TRISFbits.TRISF0

#define GPU_INT_PIN                 PORTFbits.F4
#define GPU_INT_PIN_DIRECTION       TRISFbits.TRISF4

#define GPU_POWER_DOWN_PIN          LATFbits.LATF5
#define GPU_POWER_DOWN              (GPU_POWER_DOWN_PIN = 0)
#define GPU_POWER_UP                (GPU_POWER_DOWN_PIN = 1)
#define GPU_POWER_DOWN_PIN_DIRECTION TRISFbits.TRISF5


//
// Test points
//
#define TP7_PIN                     LATBbits.LATB6
#define TP7_PIN_DIRECTION           TRISBbits.TRISB6

#define TP8_PIN                     LATBbits.LATB8
#define TP8_PIN_DIRECTION           TRISBbits.TRISB8

#define TP9_PIN                     LATEbits.LATE6
#define TP9_PIN_DIRECTION           TRISEbits.TRISE6

#define TP10_PIN                    LATBbits.LATB4
#define TP10_PIN_DIRECTION          TRISBbits.TRISB4

//
//  Unconnected pins
//
#define NC0_PIN                     LATBbits.LATB2
#define NC0_PIN_DIRECTION           TRISBbits.TRISB2

#define NC1_PIN                     LATBbits.LATB11
#define NC1_PIN_DIRECTION           TRISBbits.TRISB11

#define NC2_PIN                     LATBbits.LATB12
#define NC2_PIN_DIRECTION           TRISBbits.TRISB12

#define NC3_PIN                     LATBbits.LATB13
#define NC3_PIN_DIRECTION           TRISBbits.TRISB13

#define NC4_PIN                     LATFbits.LATF1
#define NC4_PIN_DIRECTION           TRISFbits.TRISF1

#define NC5_PIN                     LATFbits.LATF3
#define NC5_PIN_DIRECTION           TRISFbits.TRISF3

// =============================================================================
// Global variable declarations
// =============================================================================

// =============================================================================
// Public function declarations
// =============================================================================


#ifdef	__cplusplus
}
#endif

#endif	/* PINMAP_H */

