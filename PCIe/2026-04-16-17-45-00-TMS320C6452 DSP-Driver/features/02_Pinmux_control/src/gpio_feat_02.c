#include "../include/gpio_feat_02.h"
#include <stdio.h>

tms_gpio_status_t gpio_feat02_validate_pinmux(tms_gpio_ctx_t *ctx)
{
    tms_gpio_status_t st = tms_gpio_feature_pinmux_validate(ctx);
    printf("[FEAT02] Pinmux validate -> %s\n", (st==TMS_GPIO_OK)?"OK":"FAIL");
    return st;
}
