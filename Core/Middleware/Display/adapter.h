/*
 * STM32 HAL Adapter Header for 7x10 B Click Library
 *
 * 22.01.2026
 * edited by Arthur Burjak
 */

#ifndef ADAPTER_H
#define ADAPTER_H

#include "stm32l4xx_hal.h"  // STM32L4 HAL

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize 7x10 B display for STM32
 * @note Call this after initializing SPI and GPIO in STM32CubeMX
 */
void c7x10b_init(void);

// Delay functions
void Delay_1us(void);
void Delay_1ms(void);

#ifdef __cplusplus
}
#endif

#endif /* ADAPTER_H */
