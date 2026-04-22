#include "tms320c6452_dsp_write_output_out_data01.h"

/* Full memory barrier */
static inline void tms320c6452_out_mb(void) { __sync_synchronize(); }

/* Big-endian 32-bit MMIO accessors (assumes BE-mapped peripheral) */
static inline uint32_t reg_read32_be(volatile uint32_t *addr)
{
    uint32_t v = *addr;
    tms320c6452_out_mb();
    return v;
}

static inline void reg_write32_be(volatile uint32_t *addr, uint32_t v)
{
    tms320c6452_out_mb();
    *addr = v;
    tms320c6452_out_mb();
}

static int out_check_ctx(const tms320c6452_dsp_out_ctx_t *ctx)
{
    return (ctx != 0 && ctx->ready == 1u) ? 1 : 0;
}

static int out_check_pin(uint32_t pin)
{
    return (pin < 32u) ? 1 : 0;
}

int tms320c6452_dsp_write_output_out_data01_init(tms320c6452_dsp_out_ctx_t *ctx, uintptr_t base)
{
    if (ctx == 0) {
        return TMS320C6452_DSP_OUT_EINVAL;
    }
    ctx->base = (base == 0u) ? (uintptr_t)TMS320C6452_DSP_GPIO_BASE_DEFAULT : base;
    ctx->ready = 1u;
    return TMS320C6452_DSP_OUT_OK;
}

int tms320c6452_dsp_write_output_out_data01_deinit(tms320c6452_dsp_out_ctx_t *ctx)
{
    if (ctx == 0) {
        return TMS320C6452_DSP_OUT_EINVAL;
    }
    ctx->ready = 0u;
    ctx->base = 0u;
    return TMS320C6452_DSP_OUT_OK;
}

int tms320c6452_dsp_write_output_out_data01_write_pin(tms320c6452_dsp_out_ctx_t *ctx, uint32_t pin, uint32_t value)
{
    volatile uint32_t *out_reg;
    uint32_t v;
    uint32_t mask;

    if (!out_check_ctx(ctx)) { return TMS320C6452_DSP_OUT_ESTATE; }
    if (!out_check_pin(pin)) { return TMS320C6452_DSP_OUT_EINVAL; }

    out_reg = (volatile uint32_t *)((uintptr_t)ctx->base + TMS320C6452_DSP_GPIO_OUT_DATA01_OFFSET);
    v = reg_read32_be(out_reg);
    mask = TMS320C6452_DSP_GPIO_PIN_MASK(pin);

    if ((value & 0x1u) != 0u) {
        v |= mask;
    } else {
        v &= ~mask;
    }

    reg_write32_be(out_reg, v);
    return TMS320C6452_DSP_OUT_OK;
}

int tms320c6452_dsp_write_output_out_data01_read_pin(tms320c6452_dsp_out_ctx_t *ctx, uint32_t pin, uint32_t *value)
{
    volatile uint32_t *out_reg;
    uint32_t v;

    if (!out_check_ctx(ctx) || value == 0) { return TMS320C6452_DSP_OUT_EINVAL; }
    if (!out_check_pin(pin)) { return TMS320C6452_DSP_OUT_EINVAL; }

    out_reg = (volatile uint32_t *)((uintptr_t)ctx->base + TMS320C6452_DSP_GPIO_OUT_DATA01_OFFSET);
    v = reg_read32_be(out_reg);
    *value = (v >> pin) & 0x1u;
    return TMS320C6452_DSP_OUT_OK;
}

int tms320c6452_dsp_write_output_out_data01_write_all(tms320c6452_dsp_out_ctx_t *ctx, uint32_t out_value)
{
    volatile uint32_t *out_reg;

    if (!out_check_ctx(ctx)) { return TMS320C6452_DSP_OUT_ESTATE; }

    out_reg = (volatile uint32_t *)((uintptr_t)ctx->base + TMS320C6452_DSP_GPIO_OUT_DATA01_OFFSET);
    reg_write32_be(out_reg, out_value);
    return TMS320C6452_DSP_OUT_OK;
}

int tms320c6452_dsp_write_output_out_data01_read_all(tms320c6452_dsp_out_ctx_t *ctx, uint32_t *out_value)
{
    volatile uint32_t *out_reg;

    if (!out_check_ctx(ctx) || out_value == 0) { return TMS320C6452_DSP_OUT_EINVAL; }

    out_reg = (volatile uint32_t *)((uintptr_t)ctx->base + TMS320C6452_DSP_GPIO_OUT_DATA01_OFFSET);
    *out_value = reg_read32_be(out_reg);
    return TMS320C6452_DSP_OUT_OK;
}

int tms320c6452_dsp_write_output_out_data01_update_mask(tms320c6452_dsp_out_ctx_t *ctx, uint32_t mask, uint32_t value)
{
    volatile uint32_t *out_reg;
    uint32_t v;

    if (!out_check_ctx(ctx)) { return TMS320C6452_DSP_OUT_ESTATE; }

    out_reg = (volatile uint32_t *)((uintptr_t)ctx->base + TMS320C6452_DSP_GPIO_OUT_DATA01_OFFSET);
    v = reg_read32_be(out_reg);

    v = (v & ~mask) | (value & mask);

    reg_write32_be(out_reg, v);
    return TMS320C6452_DSP_OUT_OK;
}
