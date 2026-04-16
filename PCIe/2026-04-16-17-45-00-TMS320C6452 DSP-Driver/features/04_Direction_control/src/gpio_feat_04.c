#include "../include/gpio_feat_04.h"
#include <stdio.h>

tms_gpio_status_t gpio_feat04_set_get_dir(tms_gpio_ctx_t *ctx, uint32_t pin, bool input)
{
    bool got;
    if (tms_gpio_set_direction(ctx, pin, input) != TMS_GPIO_OK) { printf("[FEAT04] set DIR failed\n"); return TMS_GPIO_ESTATE; }
    if (tms_gpio_get_direction(ctx, pin, &got) != TMS_GPIO_OK) { printf("[FEAT04] get DIR failed\n"); return TMS_GPIO_ESTATE; }
    printf("[FEAT04] pin %lu DIR=%s (readback=%s)\n", (unsigned long)pin, input?"IN":"OUT", got?"IN":"OUT");
    return (got==input)?TMS_GPIO_OK:TMS_GPIO_ESTATE;
}
