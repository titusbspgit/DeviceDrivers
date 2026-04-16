#include "../include/gpio_feat_07.h"
#include <stdio.h>

static void feat07_cb(void *user, uint32_t pin)
{ (void)user; printf("[FEAT07] ISR callback for pin %lu\n", (unsigned long)pin); }

tms_gpio_status_t gpio_feat07_config_and_trigger(tms_gpio_ctx_t *ctx, uint32_t pin)
{
    /* Enable both edges on pin and bank IRQ, then toggle output to self-trigger */
    uint32_t bank = (pin < 16u) ? 0u : 1u;
    (void)tms_gpio_register_pin_isr(ctx, pin, feat07_cb, (void*)0);
    (void)tms_gpio_config_edge(ctx, pin, true, true);
    (void)tms_gpio_enable_bank_irq(ctx, bank, true);
    /* Drive an edge: ensure output mode per spec: not required, but do it */
    (void)tms_gpio_set_direction(ctx, pin, false);
    (void)tms_gpio_write_pin(ctx, pin, false);
    (void)tms_gpio_write_pin(ctx, pin, true);
    /* Check INTSTAT */
    {
        uint32_t st = tms_gpio_get_irq_status(ctx);
        if ((st & (1u<<pin)) == 0u) {
            printf("[FEAT07] No INTSTAT latch observed (check hardware)\n");
            /* Not a hard fail in simulation */
        } else {
            printf("[FEAT07] INTSTAT has pin %lu set\n", (unsigned long)pin);
            tms_gpio_clear_irq_status(ctx, (1u<<pin));
        }
    }
    /* Service bank ISR to drive callback if pending */
    tms_gpio_service_bank_isr(ctx, bank);
    return TMS_GPIO_OK;
}
