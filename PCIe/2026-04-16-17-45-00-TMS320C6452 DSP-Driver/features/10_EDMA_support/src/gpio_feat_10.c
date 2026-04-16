#include "../include/gpio_feat_10.h"
#include <stdio.h>

tms_gpio_status_t gpio_feat10_prepare_edma_use(tms_gpio_ctx_t *ctx, uint32_t bank)
{
    /* Driver only gates events via BINTEN; EDMA routing is platform-specific */
    if (tms_gpio_enable_bank_irq(ctx, bank, true) != TMS_GPIO_OK) { return TMS_GPIO_ESTATE; }
    printf("[FEAT10] Enabled BINTEN for bank %lu; route EDMA externally.\n", (unsigned long)bank);
    return TMS_GPIO_OK;
}
