#include "../include/gpio_feat_06.h"
#include <stdio.h>

tms_gpio_status_t gpio_feat06_sample_pin(tms_gpio_ctx_t *ctx, uint32_t pin, bool *level)
{
    uint32_t v;
    if ((level==(bool *)0) || (pin>=32u)) { return TMS_GPIO_EINVAL; }
    (void)tms_gpio_set_direction(ctx, pin, true);
    v = tms_gpio_read_inputs(ctx);
    *level = ((v & (1u<<pin))!=0u);
    printf("[FEAT06] Pin %lu level=%u\n", (unsigned long)pin, *level?1u:0u);
    return TMS_GPIO_OK;
}
