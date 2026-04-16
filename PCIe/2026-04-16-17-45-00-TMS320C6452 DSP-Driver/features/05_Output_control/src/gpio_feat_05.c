#include "../include/gpio_feat_05.h"
#include <stdio.h>

tms_gpio_status_t gpio_feat05_toggle_pin(tms_gpio_ctx_t *ctx, uint32_t pin)
{
    if (tms_gpio_set_direction(ctx, pin, false) != TMS_GPIO_OK) { return TMS_GPIO_ESTATE; }
    (void)tms_gpio_write_pin(ctx, pin, true);
    (void)tms_gpio_write_pin(ctx, pin, false);
    printf("[FEAT05] Toggled pin %lu via SET/CLR\n", (unsigned long)pin);
    return TMS_GPIO_OK;
}
