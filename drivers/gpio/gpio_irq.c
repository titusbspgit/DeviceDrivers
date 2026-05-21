#include "gpio_irq.h"
#include <assert.h>
#include "../../include/ag_be_io.h"

#define GPIO_PIN_MASK(pin) (1u << ((pin) & 31u))

static inline uint32_t rd(const gpio_irq_t *c, uint32_t off) {
    return readl_be((volatile void *)(c->base + off));
}
static inline void wr(const gpio_irq_t *c, uint32_t off, uint32_t v) {
    writel_be((volatile void *)(c->base + off), v);
}

void gpio_irq_configure(const gpio_irq_t *ctx, uint32_t pin, bool rising, bool falling) {
    assert(ctx);
    assert(pin < 32u);
    uint32_t m = GPIO_PIN_MASK(pin);
    if (rising)  wr(ctx, ctx->set_ris_off, m); else wr(ctx, ctx->clr_ris_off, m);
    if (falling) wr(ctx, ctx->set_fal_off, m); else wr(ctx, ctx->clr_fal_off, m);
}

void gpio_irq_clear(const gpio_irq_t *ctx, uint32_t pin) {
    assert(ctx);
    assert(pin < 32u);
    // Writing '1' to INSTAT bit typically clears latched edge; integrator must confirm on target SoC.
    writel_be((volatile void *)(ctx->base + ctx->instat_off), GPIO_PIN_MASK(pin));
}

uint32_t gpio_irq_bank_status(const gpio_irq_t *ctx) {
    assert(ctx);
    return rd(ctx, ctx->intstat01_off);
}

static void gpio_isr_trampoline(void *arg) {
    gpio_irq_t *ctx = (gpio_irq_t *)arg;
    // Read pending
    uint32_t pending = rd(ctx, ctx->instat_off);
    if (pending && ctx->cb) {
        for (uint32_t pin = 0; pin < 32u; ++pin) {
            uint32_t m = GPIO_PIN_MASK(pin);
            if (pending & m) {
                ctx->cb(pin, ctx->cb_user);
                // Clear after callback
                writel_be((volatile void *)(ctx->base + ctx->instat_off), m);
            }
        }
    }
}

bool gpio_irq_install_isr(gpio_irq_t *ctx) {
    assert(ctx);
    if (!ctx->irq_register || ctx->direct_irq_num < 0) return false;
    return ctx->irq_register(ctx->direct_irq_num, gpio_isr_trampoline, ctx);
}
