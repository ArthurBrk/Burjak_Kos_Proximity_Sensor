//
// Created by VolkerTenta on 12/12/2023.
//

#include "printf.h"

UART_HandleTypeDef *current_UART; // default

//Retargeting so the printf function is usable
PUTCHAR_PROTOTYPE {
    HAL_UART_Transmit(current_UART, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}

void set_UART(UART_HandleTypeDef *uart) {
    current_UART = uart;
}
