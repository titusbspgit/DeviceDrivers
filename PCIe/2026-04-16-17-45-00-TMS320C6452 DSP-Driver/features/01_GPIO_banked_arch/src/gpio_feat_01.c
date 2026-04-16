#include "../include/gpio_feat_01.h"
#include <stdio.h>

tms_gpio_status_t gpio_feat01_run_demo(tms_gpio_ctx_t *ctx)
{
    uint32_t b0 = tms_gpio_feature_banked_arch_get_bank_mask(0u);
    uint32_t b1 = tms_gpio_feature_banked_arch_get_bank_mask(1u);
    printf("[FEAT01] Bank0 mask=0x%08lX, Bank1 mask=0x%08lX\n", (unsigned long)b0, (unsigned long)b1);
    return TMS_GPIO_OK;
}
