#ifndef STM32_UART_H
#define STM32_UART_H

#include "main.h"
#include <stdint.h>

typedef enum
{
    STM32_UART_OK = 0,
    STM32_UART_ERROR
} stm32_uart_status_t;

stm32_uart_status_t stm32_uart_send(
    UART_HandleTypeDef *huart,
    uint8_t *data,
    uint16_t length,
    uint32_t timeout
);

stm32_uart_status_t stm32_uart_receive_it(
    UART_HandleTypeDef *huart,
    uint8_t *data,
    uint16_t length
);

#endif
