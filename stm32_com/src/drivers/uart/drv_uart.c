#include "drv_uart.h"
#include "stm32_uart.h"

#define DRV_UART_TX_TIMEOUT_MS 100U

static UART_HandleTypeDef *drv_uart_device_handle = NULL;
static UART_HandleTypeDef *drv_uart_commander_handle = NULL;

static uint8_t drv_uart_device_rx_byte = 0;
static uint8_t drv_uart_commander_rx_byte = 0;

static drv_uart_status_t drv_uart_send_raw(
    UART_HandleTypeDef *huart,
    uint8_t *data,
    uint16_t length
)
{
    if (huart == NULL || data == NULL || length == 0)
    {
        return DRV_UART_ERROR;
    }

    if (stm32_uart_send(huart, data, length, DRV_UART_TX_TIMEOUT_MS) == STM32_UART_OK)
    {
        return DRV_UART_OK;
    }

    return DRV_UART_ERROR;
}

int drv_uart_init(drv_uart_config_t *config)
{
    if (config == NULL)
    {
        return -1;
    }

    if (config->device_huart == NULL || config->commander_huart == NULL)
    {
        return -1;
    }

    drv_uart_device_handle = config->device_huart;
    drv_uart_commander_handle = config->commander_huart;

    drv_uart_start_device_receive_it();
    drv_uart_start_receive_it();

    return 0;
}

/*
 * Commander UART.
 * Ví dụ: UART2.
 */
drv_uart_status_t drv_uart_send(uint8_t *data, uint16_t length)
{
    return drv_uart_send_raw(
        drv_uart_commander_handle,
        data,
        length
    );
}

/*
 * Device UART.
 * Ví dụ: UART1.
 * Hàm này chỉ gửi raw frame đã được build sẵn.
 */
drv_uart_status_t drv_uart_device_send_raw(uint8_t *data, uint16_t length)
{
    return drv_uart_send_raw(
        drv_uart_device_handle,
        data,
        length
    );
}

void drv_uart_start_receive_it(void)
{
    if (drv_uart_commander_handle != NULL)
    {
        stm32_uart_receive_it(
            drv_uart_commander_handle,
            &drv_uart_commander_rx_byte,
            1
        );
    }
}

uint8_t drv_uart_get_rx_byte(void)
{
    return drv_uart_commander_rx_byte;
}

void drv_uart_start_device_receive_it(void)
{
    if (drv_uart_device_handle != NULL)
    {
        stm32_uart_receive_it(
            drv_uart_device_handle,
            &drv_uart_device_rx_byte,
            1
        );
    }
}

uint8_t drv_uart_get_device_rx_byte(void)
{
    return drv_uart_device_rx_byte;
}
