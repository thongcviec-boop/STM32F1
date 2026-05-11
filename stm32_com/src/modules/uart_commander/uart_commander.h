#ifndef UART_COMMANDER_H
#define UART_COMMANDER_H

#include <stdint.h>

int uart_commander_init(void);
void uart_commander_run(void);
void uart_commander_rx_callback(uint8_t data);
void uart_commander_send_string(const char *str);

#endif
