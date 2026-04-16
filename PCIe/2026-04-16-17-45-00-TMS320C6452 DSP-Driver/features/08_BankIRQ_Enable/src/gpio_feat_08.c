#include "../include/gpio_feat_08.h"
#include <stdio.h>

tms_gpio_status_t gpio_feat08_bank_irq_enable_disable(tms_gpio_ctx_t *ctx)
{
    if (tms_gpio_enable_bank_irq(ctx, 0u, true) != TMS_GPIO_OK) { return TMS_GPIO_ESTATE; }
    if (tms_gpio_enable_bank_irq(ctx, 1u, true) != TMS_GPIO_OK) { return TMS_GPIO_ESTATE; }
    printf("[FEAT08] Enabled IRQ for both banks\n");
    if (tms_gpio_enable_bank_irq(ctx, 0u, false) != TMS_GPIO_OK) { return TMS_GPIO_ESTATE; }
    if (tms_gpio_enable_bank_irq(ctx, 1u, false) != TMS_GPIO_OK) { return TMS_GPIO_ESTATE; }
    printf("[FEAT08] Disabled IRQ for both banks\n");
    return TMS_GPIO_OK;
}
