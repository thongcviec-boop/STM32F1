#include "app_main.h"

#include <stdio.h>

#include "main.h"
#include "drv_led.h"
#include "drv_uart.h"
#include "device_protocol.h"
#include "uart_commander.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

/*
 * UART1 parser:
 * UART1 nhận frame chuẩn từ device.
 */
static device_protocol_parser_t app_uart1_protocol_parser;

/*
 * UART2 parser:
 * UART2 cũng nhận frame chuẩn.
 * Không nhận raw 2 byte nữa.
 */
static device_protocol_parser_t app_uart2_protocol_parser;

static volatile uint8_t app_uart1_frame_ready = 0U;
static device_protocol_frame_t app_uart1_rx_frame;

static volatile uint8_t app_uart2_frame_ready = 0U;
static device_protocol_frame_t app_uart2_rx_frame;

/*
 * Sau khi nhận CONTROL_LED từ UART1,
 * LED blink xong thì gửi STATUS frame ra UART1.
 */
static volatile uint8_t app_wait_led_done_send_status = 0U;

static void app_main_send_status_frame_to_uart1(void)
{
    uint8_t tx_frame[DEVICE_PROTOCOL_MAX_FRAME_LENGTH];
    uint16_t tx_frame_length = 0U;

    device_protocol_status_t status = device_protocol_build_frame(
        DEVICE_PROTOCOL_MSG_STATUS_CONTROL,
        NULL,
        0U,
        tx_frame,
        sizeof(tx_frame),
        &tx_frame_length
    );

    if (status == DEVICE_PROTOCOL_OK)
    {
        drv_uart_device_send_raw(tx_frame, tx_frame_length);
    }
}

static void app_main_send_control_led_frame_to_uart1(uint8_t hz, uint8_t count)
{
    uint8_t payload[DEVICE_PROTOCOL_LED_PAYLOAD_LENGTH];
    uint8_t tx_frame[DEVICE_PROTOCOL_MAX_FRAME_LENGTH];
    uint16_t tx_frame_length = 0U;

    payload[DEVICE_PROTOCOL_LED_HZ] = hz;
    payload[DEVICE_PROTOCOL_LED_COUNT] = count;

    device_protocol_status_t status = device_protocol_build_frame(
        DEVICE_PROTOCOL_MSG_CONTROL_LED,
        payload,
        DEVICE_PROTOCOL_LED_PAYLOAD_LENGTH,
        tx_frame,
        sizeof(tx_frame),
        &tx_frame_length
    );

    if (status == DEVICE_PROTOCOL_OK)
    {
        drv_uart_device_send_raw(tx_frame, tx_frame_length);
    }
}

static void app_main_handle_frame(const device_protocol_frame_t *frame)
{
    if (frame == NULL)
    {
        return;
    }

    switch (frame->message)
    {
        case DEVICE_PROTOCOL_MSG_CONTROL_LED:
        {
            /*
             * Frame CONTROL_LED thường nhận từ UART1.
             * Sau khi nhận thì LED blink.
             * Blink xong thì gửi STATUS frame ra UART1.
             */
            if (frame->payload_length == DEVICE_PROTOCOL_LED_PAYLOAD_LENGTH)
            {
                uint8_t hz = frame->payload[DEVICE_PROTOCOL_LED_HZ];
                uint8_t count = frame->payload[DEVICE_PROTOCOL_LED_COUNT];

                if (drv_led_blink_start(DRV_LED_STATUS, hz, count) == DRV_LED_OK)
                {
                    app_wait_led_done_send_status = 1U;
                }
                else
                {
                    app_wait_led_done_send_status = 0U;
                }
            }

            break;
        }

        case DEVICE_PROTOCOL_MSG_STATUS_CONTROL:
        {
            /*
             * Giữ logic debug cũ.
             * Nếu nhận STATUS_CONTROL thì in done ra UART2.
             */
            uart_commander_send_string("\rNode 2: Done\r\n");

            //drv_led_blink_start(DRV_LED_STATUS, 2U, 5U);

            break;
        }

        case DEVICE_PROTOCOL_MSG_SERIAL_CONTROL:
        {
            /*
             * Frame SERIAL_CONTROL thường nhận từ UART2.
             *
             * Payload:
             * payload[0] = hz
             * payload[1] = count
             *
             * Sau khi nhận SERIAL_CONTROL, build frame CONTROL_LED chuẩn
             * rồi gửi ra UART1.
             */
        	if (frame->payload_length == DEVICE_PROTOCOL_LED_PAYLOAD_LENGTH)
        	{
        	    uint8_t hz = frame->payload[DEVICE_PROTOCOL_LED_HZ];
        	    uint8_t count = frame->payload[DEVICE_PROTOCOL_LED_COUNT];

        	    app_main_send_control_led_frame_to_uart1(hz, count);

        	    char log_msg[64];

        	    snprintf(
        	        log_msg,
        	        sizeof(log_msg),
        	        "\rNode 1: Send hz = %u, count = %u\r\n",
        	        hz,
        	        count
        	    );

        	    uart_commander_send_string(log_msg);
        	}

            break;
        }

        default:
            break;
    }
}

void app_main_init(void)
{
    drv_led_init();

    device_protocol_parser_init(&app_uart1_protocol_parser);
    device_protocol_parser_init(&app_uart2_protocol_parser);

    drv_uart_config_t uart_config =
    {
        .device_huart = &huart1,
        .commander_huart = &huart2
    };

    drv_uart_init(&uart_config);

    /*
     * Vẫn giữ uart_commander_init() vì còn dùng
     * uart_commander_send_string() để debug TX ra UART2.
     */
    uart_commander_init();
}

void app_main_loop(void)
{
    drv_led_process();

    if (app_wait_led_done_send_status != 0U)
    {
        if (drv_led_blink_is_done(DRV_LED_STATUS) != 0U)
        {
            app_wait_led_done_send_status = 0U;

            drv_led_blink_clear_done(DRV_LED_STATUS);

            app_main_send_status_frame_to_uart1();
        }
    }

    uart_commander_run();

    if (app_uart1_frame_ready != 0U)
    {
        device_protocol_frame_t frame;

        __disable_irq();
        frame = app_uart1_rx_frame;
        app_uart1_frame_ready = 0U;
        __enable_irq();

        app_main_handle_frame(&frame);
    }

    if (app_uart2_frame_ready != 0U)
    {
        device_protocol_frame_t frame;

        __disable_irq();
        frame = app_uart2_rx_frame;
        app_uart2_frame_ready = 0U;
        __enable_irq();

        app_main_handle_frame(&frame);
    }
}

void app_main_uart1_rx_callback(uint8_t data)
{
    device_protocol_frame_t frame;

    device_protocol_rx_result_t result = device_protocol_parse_byte(
        &app_uart1_protocol_parser,
        data,
        &frame
    );

    if (result == DEVICE_PROTOCOL_RX_FRAME_OK)
    {
        if (app_uart1_frame_ready == 0U)
        {
            app_uart1_rx_frame = frame;
            app_uart1_frame_ready = 1U;
        }
    }
}

void app_main_uart2_rx_callback(uint8_t data)
{
    device_protocol_frame_t frame;

    device_protocol_rx_result_t result = device_protocol_parse_byte(
        &app_uart2_protocol_parser,
        data,
        &frame
    );

    if (result == DEVICE_PROTOCOL_RX_FRAME_OK)
    {
        if (app_uart2_frame_ready == 0U)
        {
            app_uart2_rx_frame = frame;
            app_uart2_frame_ready = 1U;
        }
    }
}
