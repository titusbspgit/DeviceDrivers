#include "../include/gpio_feat_14.h"
#include <stdio.h>

tms_gpio_status_t gpio_feat14_full_init(tms_gpio_ctx_t *ctx)
{
    tms_gpio_status_t st = tms_gpio_feature_full_init(ctx);
    printf("[FEAT14] Full init -> %s\n", (st==TMS_GPIO_OK)?"OK":"FAIL");
    return st;
}
