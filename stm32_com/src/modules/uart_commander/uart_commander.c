#include "uart_commander.h"
#include "drv_uart.h"
#include <stdio.h>
#include <string.h>

int uart_commander_init(void)
{
    uart_commander_send_string("uart_debug started\r\n");
    uart_commander_send_string("Send raw bytes and receive debug text in UART2.\r\n");

    return 0;
}

void uart_commander_run(void)
{
    /*
     * Hiện tại chưa cần xử lý trong loop.
     * UART2 dữ liệu đến sẽ được xử lý từ interrupt callback.
     */
}

void uart_commander_send_string(const char *str)
{
    if (str != NULL)
    {
        drv_uart_send((uint8_t *)str, strlen(str));
    }
}

void uart_commander_rx_callback(uint8_t data)
{
    char debug_message[48];

    if (data >= 32 && data <= 126)
    {
        snprintf(debug_message, sizeof(debug_message), "RX: 0x%02X ('%c')\r\n", data, data);
    }
    else
    {
        snprintf(debug_message, sizeof(debug_message), "RX: 0x%02X\r\n", data);
    }

    uart_commander_send_string(debug_message);
}
