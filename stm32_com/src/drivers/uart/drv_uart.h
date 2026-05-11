#ifndef DRV_UART_H
#define DRV_UART_H

#include "main.h"
#include <stdint.h>

typedef enum
{
    DRV_UART_OK = 0,
    DRV_UART_ERROR
} drv_uart_status_t;

typedef struct
{
    /*
     * UART dùng để giao tiếp với thiết bị khác.
     * Ví dụ: huart1
     */
    UART_HandleTypeDef *device_huart;

    /*
     * UART dùng cho commander/debug.
     * Ví dụ: huart2
     */
    UART_HandleTypeDef *commander_huart;

} drv_uart_config_t;

int drv_uart_init(drv_uart_config_t *config);

/*
 * Gửi raw text qua commander UART.
 */
drv_uart_status_t drv_uart_send(uint8_t *data, uint16_t length);

/*
 * Gửi raw data qua device UART.
 * Protocol frame sẽ được build ở device_protocol.c.
 */
drv_uart_status_t drv_uart_device_send_raw(uint8_t *data, uint16_t length);

/*
 * Receive cho commander UART.
 */
void drv_uart_start_receive_it(void);
uint8_t drv_uart_get_rx_byte(void);

/*
 * Receive cho device UART.
 */
void drv_uart_start_device_receive_it(void);
uint8_t drv_uart_get_device_rx_byte(void);

#endif /* DRV_UART_H */
