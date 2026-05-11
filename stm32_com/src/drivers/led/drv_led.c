#include "drv_led.h"

#include "board_config.h"
#include "stm32_gpio.h"
#include "main.h"

typedef struct
{
    GPIO_TypeDef *port;
    uint16_t pin;
    uint8_t active_low;
} drv_led_config_t;

typedef struct
{
    uint8_t active;
    uint8_t is_on;
    uint8_t done;
    uint16_t target_count;
    uint16_t done_count;
    uint32_t half_period_ms;
    uint32_t last_tick;
} drv_led_blink_state_t;

static const drv_led_config_t led_table[DRV_LED_MAX] =
{
    [DRV_LED_STATUS] =
    {
        .port = BOARD_LED_STATUS_PORT,
        .pin = BOARD_LED_STATUS_PIN,
        .active_low = BOARD_LED_STATUS_ACTIVE_LOW
    }
};

static drv_led_blink_state_t led_blink_state[DRV_LED_MAX];

static uint8_t drv_led_is_valid(drv_led_id_t led)
{
    return (led < DRV_LED_MAX);
}

static void drv_led_write(drv_led_id_t led, uint8_t state)
{
    if (!drv_led_is_valid(led))
    {
        return;
    }

    if (state != 0U)
    {
        if (led_table[led].active_low)
        {
            stm32_gpio_write(
                led_table[led].port,
                led_table[led].pin,
                GPIO_PIN_RESET
            );
        }
        else
        {
            stm32_gpio_write(
                led_table[led].port,
                led_table[led].pin,
                GPIO_PIN_SET
            );
        }
    }
    else
    {
        if (led_table[led].active_low)
        {
            stm32_gpio_write(
                led_table[led].port,
                led_table[led].pin,
                GPIO_PIN_SET
            );
        }
        else
        {
            stm32_gpio_write(
                led_table[led].port,
                led_table[led].pin,
                GPIO_PIN_RESET
            );
        }
    }
}

static void drv_led_blink_clear_state(drv_led_id_t led)
{
    if (!drv_led_is_valid(led))
    {
        return;
    }

    led_blink_state[led].active = 0U;
    led_blink_state[led].is_on = 0U;
    led_blink_state[led].done = 0U;
    led_blink_state[led].target_count = 0U;
    led_blink_state[led].done_count = 0U;
    led_blink_state[led].half_period_ms = 0U;
    led_blink_state[led].last_tick = 0U;
}

int drv_led_init(void)
{
    for (uint8_t i = 0U; i < DRV_LED_MAX; i++)
    {
        drv_led_blink_clear_state((drv_led_id_t)i);
        drv_led_write((drv_led_id_t)i, 0U);
    }

    return 0;
}

void drv_led_on(drv_led_id_t led)
{
    if (!drv_led_is_valid(led))
    {
        return;
    }

    drv_led_blink_clear_state(led);
    drv_led_write(led, 1U);
}

void drv_led_off(drv_led_id_t led)
{
    if (!drv_led_is_valid(led))
    {
        return;
    }

    drv_led_blink_clear_state(led);
    drv_led_write(led, 0U);
}

void drv_led_toggle(drv_led_id_t led)
{
    if (!drv_led_is_valid(led))
    {
        return;
    }

    drv_led_blink_clear_state(led);

    stm32_gpio_toggle(
        led_table[led].port,
        led_table[led].pin
    );
}

drv_led_status_t drv_led_blink_start(drv_led_id_t led, uint16_t hz, uint16_t count)
{
    if (!drv_led_is_valid(led))
    {
        return DRV_LED_ERROR;
    }

    if ((hz == 0U) || (count == 0U))
    {
        drv_led_blink_stop(led);
        return DRV_LED_ERROR;
    }

    /*
     * hz = số chu kỳ blink trong 1 giây.
     *
     * 1 blink = ON + OFF.
     *
     * Ví dụ:
     * hz = 2
     * => 2 lần blink / giây
     * => mỗi chu kỳ blink = 500ms
     * => ON 250ms, OFF 250ms
     */
    uint32_t half_period_ms = 1000U / ((uint32_t)hz * 2U);

    if (half_period_ms == 0U)
    {
        half_period_ms = 1U;
    }

    led_blink_state[led].active = 1U;
    led_blink_state[led].is_on = 1U;
    led_blink_state[led].done = 0U;
    led_blink_state[led].target_count = count;
    led_blink_state[led].done_count = 0U;
    led_blink_state[led].half_period_ms = half_period_ms;
    led_blink_state[led].last_tick = HAL_GetTick();

    drv_led_write(led, 1U);

    return DRV_LED_OK;
}

void drv_led_blink_stop(drv_led_id_t led)
{
    if (!drv_led_is_valid(led))
    {
        return;
    }

    drv_led_blink_clear_state(led);
    drv_led_write(led, 0U);
}

void drv_led_process(void)
{
    uint32_t now = HAL_GetTick();

    for (uint8_t i = 0U; i < DRV_LED_MAX; i++)
    {
        drv_led_blink_state_t *blink = &led_blink_state[i];

        if (blink->active == 0U)
        {
            continue;
        }

        if ((now - blink->last_tick) < blink->half_period_ms)
        {
            continue;
        }

        blink->last_tick = now;

        if (blink->is_on != 0U)
        {
            /*
             * LED đang ON, bây giờ tắt LED.
             * ON -> OFF được tính là hoàn thành 1 lần blink.
             */
            drv_led_write((drv_led_id_t)i, 0U);

            blink->is_on = 0U;
            blink->done_count++;

            if (blink->done_count >= blink->target_count)
            {
                /*
                 * Không gọi drv_led_blink_stop() ở đây,
                 * vì stop sẽ clear mất cờ done.
                 */
                blink->active = 0U;
                blink->is_on = 0U;
                blink->done = 1U;

                blink->target_count = 0U;
                blink->done_count = 0U;
                blink->half_period_ms = 0U;
                blink->last_tick = 0U;

                drv_led_write((drv_led_id_t)i, 0U);
            }
        }
        else
        {
            /*
             * LED đang OFF, bật lại LED để bắt đầu lần blink tiếp theo.
             */
            drv_led_write((drv_led_id_t)i, 1U);
            blink->is_on = 1U;
        }
    }
}

uint8_t drv_led_blink_is_done(drv_led_id_t led)
{
    if (!drv_led_is_valid(led))
    {
        return 0U;
    }

    return led_blink_state[led].done;
}

void drv_led_blink_clear_done(drv_led_id_t led)
{
    if (!drv_led_is_valid(led))
    {
        return;
    }

    led_blink_state[led].done = 0U;
}
