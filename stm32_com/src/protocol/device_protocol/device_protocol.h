#ifndef DEVICE_PROTOCOL_H
#define DEVICE_PROTOCOL_H

#include <stdint.h>

#define DEVICE_PROTOCOL_START_BYTE          0xFAU
#define DEVICE_PROTOCOL_HEADER_LENGTH       3U
#define DEVICE_PROTOCOL_MAX_PAYLOAD_LENGTH  255U
#define DEVICE_PROTOCOL_MAX_FRAME_LENGTH    (DEVICE_PROTOCOL_HEADER_LENGTH + DEVICE_PROTOCOL_MAX_PAYLOAD_LENGTH)

#define DEVICE_PROTOCOL_LED_PAYLOAD_LENGTH  2U

/*
 * Frame format:
 *
 * Byte 0: Start byte = 0xFA
 * Byte 1: Message
 *         0xAA = CONTROL_LED
 *         0xBB = STATUS_CONTROL
 *         0xCC = SERIAL_CONTROL
 * Byte 2: Payload length = N
 * Byte 3...: Payload
 *
 * CONTROL_LED payload:
 * Byte 3: hz
 * Byte 4: count
 *
 * SERIAL_CONTROL payload:
 * Byte 3: hz
 * Byte 4: count
 *
 * Example:
 * FA CC 02 02 05
 * => UART2 nhận SERIAL_CONTROL, sau đó build CONTROL_LED gửi qua UART1
 *
 * CONTROL_LED gửi ra UART1:
 * FA AA 02 02 05
 */
typedef enum
{
    DEVICE_PROTOCOL_MSG_CONTROL_LED    = 0xAA,
    DEVICE_PROTOCOL_MSG_STATUS_CONTROL = 0xBB,
    DEVICE_PROTOCOL_MSG_SERIAL_CONTROL = 0xCC
} device_protocol_msg_t;

typedef enum
{
    DEVICE_PROTOCOL_LED_HZ = 0,
    DEVICE_PROTOCOL_LED_COUNT,
    DEVICE_PROTOCOL_LED_PAYLOAD_MAX
} device_protocol_led_payload_t;

typedef enum
{
    DEVICE_PROTOCOL_OK = 0,
    DEVICE_PROTOCOL_ERROR
} device_protocol_status_t;

typedef enum
{
    DEVICE_PROTOCOL_RX_NONE = 0,
    DEVICE_PROTOCOL_RX_FRAME_OK,
    DEVICE_PROTOCOL_RX_FRAME_ERROR
} device_protocol_rx_result_t;

typedef struct
{
    device_protocol_msg_t message;
    uint8_t payload_length;
    uint8_t payload[DEVICE_PROTOCOL_MAX_PAYLOAD_LENGTH];
} device_protocol_frame_t;

typedef enum
{
    DEVICE_PROTOCOL_RX_WAIT_START = 0,
    DEVICE_PROTOCOL_RX_WAIT_MESSAGE,
    DEVICE_PROTOCOL_RX_WAIT_LENGTH,
    DEVICE_PROTOCOL_RX_WAIT_PAYLOAD
} device_protocol_rx_state_t;

typedef struct
{
    device_protocol_rx_state_t state;
    uint8_t message;
    uint8_t payload_length;
    uint8_t payload_index;
    uint8_t payload[DEVICE_PROTOCOL_MAX_PAYLOAD_LENGTH];
} device_protocol_parser_t;

void device_protocol_parser_init(device_protocol_parser_t *parser);

device_protocol_status_t device_protocol_build_frame(
    device_protocol_msg_t message,
    uint8_t *payload,
    uint16_t payload_length,
    uint8_t *frame_out,
    uint16_t frame_out_size,
    uint16_t *frame_length_out
);

device_protocol_rx_result_t device_protocol_parse_byte(
    device_protocol_parser_t *parser,
    uint8_t byte,
    device_protocol_frame_t *frame_out
);

#endif /* DEVICE_PROTOCOL_H */
