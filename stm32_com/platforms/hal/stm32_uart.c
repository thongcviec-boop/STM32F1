#include "stm32_uart.h"

stm32_uart_status_t stm32_uart_send(
    UART_HandleTypeDef *huart,
    uint8_t *data,
    uint16_t length,
    uint32_t timeout
)
{
    if (huart == NULL || data == NULL || length == 0)
    {
        return STM32_UART_ERROR;
    }

    if (HAL_UART_Transmit(huart, data, length, timeout) == HAL_OK)
    {
        return STM32_UART_OK;
    }

    return STM32_UART_ERROR;
}

stm32_uart_status_t stm32_uart_receive_it(
    UART_HandleTypeDef *huart,
    uint8_t *data,
    uint16_t length
)
{
    if (huart == NULL || data == NULL || length == 0)
    {
        return STM32_UART_ERROR;
    }

    if (HAL_UART_Receive_IT(huart, data, length) == HAL_OK)
    {
        return STM32_UART_OK;
    }

    return STM32_UART_ERROR;
}
