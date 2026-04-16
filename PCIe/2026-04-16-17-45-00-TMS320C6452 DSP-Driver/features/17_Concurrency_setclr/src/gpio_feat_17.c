#include "../include/gpio_feat_17.h"
#include <stdio.h>

tms_gpio_status_t gpio_feat17_atomic_setclr(tms_gpio_ctx_t *ctx, uint32_t set_mask, uint32_t clr_mask)
{
    (void)tms_gpio_set_mask(ctx, set_mask);
    (void)tms_gpio_clear_mask(ctx, clr_mask);
    printf("[FEAT17] Atomic set=0x%08lX clr=0x%08lX\n", (unsigned long)set_mask, (unsigned long)clr_mask);
    return TMS_GPIO_OK;
}
