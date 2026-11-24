/*
 * STM32 HAL Implementation for 7x10 B Click Library
 *
 * This file replaces the MikroE HAL with STM32 HAL implementations.
 * Copy this file to your project and include it instead of modifying the original HAL.
 *
 * 22.01.2026
 * edited by Arthur Burjak
 */

#include "stm32l4xx_hal.h"  // STM32L4 HAL
#include "c7x10b_config.h"
#include "stdint.h"

// HAL Configuration - match the original HAL structure
#define __HAL_SPI__                    // Enable SPI HAL
#define __AN_PIN_OUTPUT__         0
#define __RST_PIN_OUTPUT__        1
#define __CS_PIN_OUTPUT__         2
#define __PWM_PIN_OUTPUT__        6

// HAL Abstract Types
#define T_HAL_P const uint8_t*
typedef void (*T_hal_gpioSetFp)(uint8_t);
typedef uint8_t (*T_hal_gpioGetFp)();

// External SPI handle - using SPI1
extern SPI_HandleTypeDef hspi1;

// Forward declarations
static void hal_gpio_anSet(uint8_t state);
static void hal_gpio_rstSet(uint8_t state);
static void hal_gpio_csSet(uint8_t state);
static void hal_gpio_pwmSet(uint8_t state);
static void hal_spiMap(T_HAL_P spiObj);
static void hal_spiWrite(uint8_t *pBuf, uint16_t nBytes);
static void hal_gpioMap(T_HAL_P gpioObj);

/* ============================================================================
 * GPIO Implementations
 * ============================================================================ */

static void hal_gpio_anSet(uint8_t state)
{
    HAL_GPIO_WritePin(AN_PIN_PORT, AN_PIN_NUMBER, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

static void hal_gpio_rstSet(uint8_t state)
{
    HAL_GPIO_WritePin(RST_PIN_PORT, RST_PIN_NUMBER, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

static void hal_gpio_csSet(uint8_t state)
{
    HAL_GPIO_WritePin(CS_PIN_PORT, CS_PIN_NUMBER, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

static void hal_gpio_pwmSet(uint8_t state)
{
    HAL_GPIO_WritePin(PWM_PIN_PORT, PWM_PIN_NUMBER, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

/* ============================================================================
 * SPI Implementations
 * ============================================================================ */

static void hal_spiMap(T_HAL_P spiObj)
{
    // Not needed for STM32 - SPI handle is global
    (void)spiObj;
}

static void hal_spiWrite(uint8_t *pBuf, uint16_t nBytes)
{
    // CS is controlled manually in the driver, so we don't toggle it here
    // The driver will call hal_gpio_csSet before/after this function
    HAL_SPI_Transmit(&hspi1, pBuf, nBytes, HAL_MAX_DELAY);
}

static void hal_gpioMap(T_HAL_P gpioObj)
{
    // Not needed for STM32 - GPIO pins are directly accessed
    (void)gpioObj;
}

/* ============================================================================
 * Delay Implementations (inline versions)
 * ============================================================================ */

static inline void Delay_1us(void)
{
    // Simple delay loop - adjust based on your CPU frequency
    // For 168MHz: ~168 cycles = 1us
    volatile uint32_t count = SystemCoreClock / 1000000 / 4;
    while(count--);
}

static inline void Delay_1ms(void)
{
    HAL_Delay(1);
}
