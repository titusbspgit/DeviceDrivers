#include "../include/gpio_feat_12.h"
#include <stdio.h>

tms_gpio_status_t gpio_feat12_low_power_cycle(tms_gpio_ctx_t *ctx)
{
    if (tms_gpio_enter_low_power(ctx) != TMS_GPIO_OK) { return TMS_GPIO_ESTATE; }
    printf("[FEAT12] Entered low power (GPIO clock gated)\n");
    if (tms_gpio_exit_low_power(ctx) != TMS_GPIO_OK) { return TMS_GPIO_ESTATE; }
    printf("[FEAT12] Exited low power (GPIO clock re-enabled)\n");
    return TMS_GPIO_OK;
}
