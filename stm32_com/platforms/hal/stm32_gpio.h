#ifndef STM32_GPIO_H
#define STM32_GPIO_H

#include "main.h"
#include <stdint.h>

void stm32_gpio_write(GPIO_TypeDef *port, uint16_t pin, GPIO_PinState state);
void stm32_gpio_toggle(GPIO_TypeDef *port, uint16_t pin);
GPIO_PinState stm32_gpio_read(GPIO_TypeDef *port, uint16_t pin);

#endif
