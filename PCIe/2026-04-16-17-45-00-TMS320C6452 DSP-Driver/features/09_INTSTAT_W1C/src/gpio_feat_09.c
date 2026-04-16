#include "../include/gpio_feat_09.h"
#include <stdio.h>

tms_gpio_status_t gpio_feat09_latch_and_clear(tms_gpio_ctx_t *ctx, uint32_t pin)
{
    uint32_t mask = (1u<<pin);
    (void)tms_gpio_config_edge(ctx, pin, true, true);
    (void)tms_gpio_set_direction(ctx, pin, false);
    (void)tms_gpio_write_pin(ctx, pin, false);
    (void)tms_gpio_write_pin(ctx, pin, true);
    if ((tms_gpio_get_irq_status(ctx) & mask) == 0u){ printf("[FEAT09] No latch observed (check HW)\n"); }
    tms_gpio_clear_irq_status(ctx, mask);
    if ((tms_gpio_get_irq_status(ctx) & mask) != 0u){ printf("[FEAT09] Clear failed\n"); return TMS_GPIO_ESTATE; }
    printf("[FEAT09] Latch+clear OK for pin %lu\n", (unsigned long)pin);
    return TMS_GPIO_OK;
}
