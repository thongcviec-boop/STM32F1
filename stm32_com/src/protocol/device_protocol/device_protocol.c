#include "device_protocol.h"

#include <string.h>

static void device_protocol_parser_reset(device_protocol_parser_t *parser)
{
    if (parser == NULL)
    {
        return;
    }

    parser->state = DEVICE_PROTOCOL_RX_WAIT_START;
    parser->message = 0U;
    parser->payload_length = 0U;
    parser->payload_index = 0U;
}

void device_protocol_parser_init(device_protocol_parser_t *parser)
{
    device_protocol_parser_reset(parser);
}

static uint8_t device_protocol_is_valid_message(uint8_t message)
{
    switch (message)
    {
        case DEVICE_PROTOCOL_MSG_CONTROL_LED:
        case DEVICE_PROTOCOL_MSG_STATUS_CONTROL:
        case DEVICE_PROTOCOL_MSG_SERIAL_CONTROL:
            return 1U;

        default:
            return 0U;
    }
}

static uint8_t device_protocol_is_valid_led_payload(
    uint8_t *payload,
    uint8_t payload_length
)
{
    if (payload_length != DEVICE_PROTOCOL_LED_PAYLOAD_LENGTH)
    {
        return 0U;
    }

    if (payload == NULL)
    {
        return 0U;
    }

    if (payload[DEVICE_PROTOCOL_LED_HZ] == 0U)
    {
        return 0U;
    }

    if (payload[DEVICE_PROTOCOL_LED_COUNT] == 0U)
    {
        return 0U;
    }

    return 1U;
}

static uint8_t device_protocol_is_valid_payload(
    uint8_t message,
    uint8_t *payload,
    uint8_t payload_length
)
{
    switch (message)
    {
        case DEVICE_PROTOCOL_MSG_CONTROL_LED:
        {
            return device_protocol_is_valid_led_payload(payload, payload_length);
        }

        case DEVICE_PROTOCOL_MSG_SERIAL_CONTROL:
        {
            return device_protocol_is_valid_led_payload(payload, payload_length);
        }

        case DEVICE_PROTOCOL_MSG_STATUS_CONTROL:
        {
            return (payload_length == 0U);
        }

        default:
            return 0U;
    }
}

static uint8_t device_protocol_is_valid_length(
    uint8_t message,
    uint8_t payload_length
)
{
    switch (message)
    {
        case DEVICE_PROTOCOL_MSG_CONTROL_LED:
            return (payload_length == DEVICE_PROTOCOL_LED_PAYLOAD_LENGTH);

        case DEVICE_PROTOCOL_MSG_SERIAL_CONTROL:
            return (payload_length == DEVICE_PROTOCOL_LED_PAYLOAD_LENGTH);

        case DEVICE_PROTOCOL_MSG_STATUS_CONTROL:
            return (payload_length == 0U);

        default:
            return 0U;
    }
}

device_protocol_status_t device_protocol_build_frame(
    device_protocol_msg_t message,
    uint8_t *payload,
    uint16_t payload_length,
    uint8_t *frame_out,
    uint16_t frame_out_size,
    uint16_t *frame_length_out
)
{
    if ((frame_out == NULL) || (frame_length_out == NULL))
    {
        return DEVICE_PROTOCOL_ERROR;
    }

    if (payload_length > DEVICE_PROTOCOL_MAX_PAYLOAD_LENGTH)
    {
        return DEVICE_PROTOCOL_ERROR;
    }

    if ((payload_length > 0U) && (payload == NULL))
    {
        return DEVICE_PROTOCOL_ERROR;
    }

    if (!device_protocol_is_valid_message((uint8_t)message))
    {
        return DEVICE_PROTOCOL_ERROR;
    }

    if (!device_protocol_is_valid_payload(
            (uint8_t)message,
            payload,
            (uint8_t)payload_length
        ))
    {
        return DEVICE_PROTOCOL_ERROR;
    }

    if (frame_out_size < (DEVICE_PROTOCOL_HEADER_LENGTH + payload_length))
    {
        return DEVICE_PROTOCOL_ERROR;
    }

    frame_out[0] = DEVICE_PROTOCOL_START_BYTE;
    frame_out[1] = (uint8_t)message;
    frame_out[2] = (uint8_t)payload_length;

    if (payload_length > 0U)
    {
        memcpy(&frame_out[3], payload, payload_length);
    }

    *frame_length_out = DEVICE_PROTOCOL_HEADER_LENGTH + payload_length;

    return DEVICE_PROTOCOL_OK;
}

device_protocol_rx_result_t device_protocol_parse_byte(
    device_protocol_parser_t *parser,
    uint8_t byte,
    device_protocol_frame_t *frame_out
)
{
    if ((parser == NULL) || (frame_out == NULL))
    {
        return DEVICE_PROTOCOL_RX_FRAME_ERROR;
    }

    switch (parser->state)
    {
        case DEVICE_PROTOCOL_RX_WAIT_START:
        {
            if (byte == DEVICE_PROTOCOL_START_BYTE)
            {
                parser->state = DEVICE_PROTOCOL_RX_WAIT_MESSAGE;
            }

            return DEVICE_PROTOCOL_RX_NONE;
        }

        case DEVICE_PROTOCOL_RX_WAIT_MESSAGE:
        {
            if (device_protocol_is_valid_message(byte))
            {
                parser->message = byte;
                parser->state = DEVICE_PROTOCOL_RX_WAIT_LENGTH;
                return DEVICE_PROTOCOL_RX_NONE;
            }

            if (byte == DEVICE_PROTOCOL_START_BYTE)
            {
                parser->state = DEVICE_PROTOCOL_RX_WAIT_MESSAGE;
                return DEVICE_PROTOCOL_RX_NONE;
            }

            device_protocol_parser_reset(parser);
            return DEVICE_PROTOCOL_RX_FRAME_ERROR;
        }

        case DEVICE_PROTOCOL_RX_WAIT_LENGTH:
        {
            parser->payload_length = byte;
            parser->payload_index = 0U;

            if (!device_protocol_is_valid_length(parser->message, parser->payload_length))
            {
                device_protocol_parser_reset(parser);
                return DEVICE_PROTOCOL_RX_FRAME_ERROR;
            }

            if (parser->payload_length == 0U)
            {
                if (device_protocol_is_valid_payload(
                        parser->message,
                        NULL,
                        parser->payload_length
                    ))
                {
                    frame_out->message = (device_protocol_msg_t)parser->message;
                    frame_out->payload_length = parser->payload_length;

                    device_protocol_parser_reset(parser);
                    return DEVICE_PROTOCOL_RX_FRAME_OK;
                }

                device_protocol_parser_reset(parser);
                return DEVICE_PROTOCOL_RX_FRAME_ERROR;
            }

            parser->state = DEVICE_PROTOCOL_RX_WAIT_PAYLOAD;
            return DEVICE_PROTOCOL_RX_NONE;
        }

        case DEVICE_PROTOCOL_RX_WAIT_PAYLOAD:
        {
            parser->payload[parser->payload_index] = byte;
            parser->payload_index++;

            if (parser->payload_index >= parser->payload_length)
            {
                if (device_protocol_is_valid_payload(
                        parser->message,
                        parser->payload,
                        parser->payload_length
                    ))
                {
                    frame_out->message = (device_protocol_msg_t)parser->message;
                    frame_out->payload_length = parser->payload_length;

                    memcpy(
                        frame_out->payload,
                        parser->payload,
                        parser->payload_length
                    );

                    device_protocol_parser_reset(parser);
                    return DEVICE_PROTOCOL_RX_FRAME_OK;
                }

                device_protocol_parser_reset(parser);
                return DEVICE_PROTOCOL_RX_FRAME_ERROR;
            }

            return DEVICE_PROTOCOL_RX_NONE;
        }

        default:
        {
            device_protocol_parser_reset(parser);
            return DEVICE_PROTOCOL_RX_FRAME_ERROR;
        }
    }
}
