/* 
 * File:   mcu.h
 * Author: Erik
 *
 * Created on den 10 februari 2016, 22:50
 */

#ifndef MCU_H
#define	MCU_H

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

// =============================================================================
// Global constatants
// =============================================================================
#define SYSCLK_FREQ_HZ  ((uint32_t)200000000)
#define PBDIV_VALUE     2
#define PBCLK_FREQ_HZ   ((uint32_t)(SYSCLK_FREQ_HZ / PBDIV_VALUE))

// =============================================================================
// Global variable declarations
// =============================================================================

// =============================================================================
// Public function declarations
// =============================================================================

/**
 * @brief Starts up and checks the state of the microcontroller.
 * @param void
 * @return void
 */
void mcu_init(void);

#ifdef	__cplusplus
}
#endif

#endif	/* MCU_H */

