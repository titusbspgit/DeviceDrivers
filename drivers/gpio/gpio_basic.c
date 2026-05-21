#include "gpio_basic.h"
#include <assert.h>
#include "../../include/ag_be_io.h"

#define GPIO_PIN_MASK(pin) (1u << ((pin) & 31u))

static inline uint32_t rd(const gpio_basic_t *c, uint32_t off) {
    return readl_be((volatile void *)(c->base + off));
}
static inline void wr(const gpio_basic_t *c, uint32_t off, uint32_t v) {
    writel_be((volatile void *)(c->base + off), v);
}

void gpio_basic_init(const gpio_basic_t *ctx) {
    assert(ctx);
    if (ctx->lpsc_enable) {
        ctx->lpsc_enable();
    }
    // No default DIR or output level changes here to avoid touching unknown reset states.
}

void gpio_set_dir(const gpio_basic_t *ctx, uint32_t pin, bool is_output) {
    assert(ctx);
    assert(pin < 32u);
    uint32_t dir = rd(ctx, ctx->dir_off);
    if (is_output) {
        dir &= ~GPIO_PIN_MASK(pin); // TI GPIO DIR usually 0=output, 1=input (conservative; integrator must verify)
    } else {
        dir |= GPIO_PIN_MASK(pin);
    }
    wr(ctx, ctx->dir_off, dir);
}

void gpio_write(const gpio_basic_t *ctx, uint32_t pin, bool high) {
    assert(ctx);
    assert(pin < 32u);
    if (high) {
        wr(ctx, ctx->set_data_off, GPIO_PIN_MASK(pin));
    } else {
        wr(ctx, ctx->clr_data_off, GPIO_PIN_MASK(pin));
    }
}

bool gpio_read(const gpio_basic_t *ctx, uint32_t pin) {
    assert(ctx);
    assert(pin < 32u);
    uint32_t v = rd(ctx, ctx->indata_off);
    return (v & GPIO_PIN_MASK(pin)) != 0u;
}
