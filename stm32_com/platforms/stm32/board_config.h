#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

#include "main.h"

/*
 * Board: STM32F103C8T6 Blue Pill
 * LED PC13 thường là active-low:
 * GPIO_PIN_RESET = ON
 * GPIO_PIN_SET   = OFF
 */

#define BOARD_LED_STATUS_PORT        GPIOC
#define BOARD_LED_STATUS_PIN         GPIO_PIN_13
#define BOARD_LED_STATUS_ACTIVE_LOW  1

#define BOARD_UART_INSTANCE          USART1

#endif
