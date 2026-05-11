#include "led_blink.h"
#include "drv_led.h"
#include "main.h"

#define LED_BLINK_INTERVAL_MS    200U

static uint32_t led_blink_last_tick = 0;
static uint8_t led_blink_enable = 0;

int led_blink_init(void)
{
    drv_led_init();

    led_blink_last_tick = HAL_GetTick();
    led_blink_enable = 1;

    return 0;
}

void led_blink_run(void)
{
    if (led_blink_enable == 0)
    {
        return;
    }

    if ((HAL_GetTick() - led_blink_last_tick) >= LED_BLINK_INTERVAL_MS)
    {
        led_blink_last_tick = HAL_GetTick();
        drv_led_toggle(DRV_LED_STATUS);
    }
}

void led_blink_stop(void)
{
    led_blink_enable = 0;
    drv_led_off(DRV_LED_STATUS);
}
