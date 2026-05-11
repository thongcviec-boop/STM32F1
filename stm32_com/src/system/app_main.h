#ifndef APP_MAIN_H
#define APP_MAIN_H

#include <stdint.h>

void app_main_init(void);
void app_main_loop(void);

void app_main_uart1_rx_callback(uint8_t data);
void app_main_uart2_rx_callback(uint8_t data);

#endif /* APP_MAIN_H */
