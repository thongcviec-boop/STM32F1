#ifndef DRV_LED_H
#define DRV_LED_H

#include <stdint.h>

typedef enum
{
    DRV_LED_STATUS = 0,
    DRV_LED_MAX
} drv_led_id_t;

typedef enum
{
    DRV_LED_OK = 0,
    DRV_LED_ERROR
} drv_led_status_t;

int drv_led_init(void);

void drv_led_on(drv_led_id_t led);
void drv_led_off(drv_led_id_t led);
void drv_led_toggle(drv_led_id_t led);

drv_led_status_t drv_led_blink_start(drv_led_id_t led, uint16_t hz, uint16_t count);
void drv_led_blink_stop(drv_led_id_t led);
void drv_led_process(void);

/*
 * Kiểm tra LED đã blink đủ count chưa.
 * Hàm này trả về 1 sau khi drv_led_process() chạy đủ count.
 */
uint8_t drv_led_blink_is_done(drv_led_id_t led);

/*
 * Clear cờ done sau khi app đã xử lý xong.
 */
void drv_led_blink_clear_done(drv_led_id_t led);

#endif /* DRV_LED_H */
