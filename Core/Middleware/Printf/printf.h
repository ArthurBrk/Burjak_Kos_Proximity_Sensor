//
// Created by VolkerTenta on 12/12/2023.
//

#ifndef PRINTF_H
#define PRINTF_H

#include "stm32l4xx_hal.h"

#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)

void set_UART(UART_HandleTypeDef *uart);

#endif //PRINTF_H
