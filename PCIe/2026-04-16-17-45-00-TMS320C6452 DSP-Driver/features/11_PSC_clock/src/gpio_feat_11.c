#include "../include/gpio_feat_11.h"
#include <stdio.h>

tms_gpio_status_t gpio_feat11_clock_enable_disable(tms_gpio_ctx_t *ctx)
{
    if (tms_gpio_enable_module(ctx) != TMS_GPIO_OK) { return TMS_GPIO_ESTATE; }
    printf("[FEAT11] Clock enabled via PSC hook\n");
    if (tms_gpio_disable_module(ctx) != TMS_GPIO_OK) { return TMS_GPIO_ESTATE; }
    printf("[FEAT11] Clock disabled via PSC hook\n");
    (void)tms_gpio_enable_module(ctx);
    return TMS_GPIO_OK;
}
