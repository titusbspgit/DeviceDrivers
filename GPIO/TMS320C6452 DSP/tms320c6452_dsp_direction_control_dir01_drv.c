/* Single-feature driver: Direction control (DIR01) */
#include "tms320c6452_dsp_direction_control_dir01.h"

/* Full memory barrier for ordering */
static inline void tms320c6452_dir_mb(void) { __sync_synchronize(); }

/* Big-endian 32-bit MMIO accessors */
static inline uint32_t reg_read32_be(volatile uint32_t *addr)
{
    uint32_t v = *addr; /* assumes BE-mapped peripheral */
    tms320c6452_dir_mb();
    return v;
}

static inline void reg_write32_be(volatile uint32_t *addr, uint32_t v)
{
    tms320c6452_dir_mb();
    *addr = v; /* assumes BE-mapped peripheral */
    tms320c6452_dir_mb();
}

/* Local checks */
static int dir01_check_ctx(const tms320c6452_dsp_dir_ctx_t *ctx)
{
    return (ctx != (const tms320c6452_dsp_dir_ctx_t *)0 && ctx->ready == 1u) ? 1 : 0;
}

static int dir01_check_pin(uint32_t pin)
{
    return (pin < 32u) ? 1 : 0;
}

int tms320c6452_dsp_direction_control_dir01_init(tms320c6452_dsp_dir_ctx_t *ctx, uintptr_t base)
{
    if (ctx == (tms320c6452_dsp_dir_ctx_t *)0) {
        return TMS320C6452_DSP_DIR_EINVAL;
    }
    ctx->base = (base == (uintptr_t)0) ? (uintptr_t)TMS320C6452_DSP_GPIO_BASE_DEFAULT : base;
    ctx->ready = 1u;
    return TMS320C6452_DSP_DIR_OK;
}

int tms320c6452_dsp_direction_control_dir01_deinit(tms320c6452_dsp_dir_ctx_t *ctx)
{
    if (ctx == (tms320c6452_dsp_dir_ctx_t *)0) {
        return TMS320C6452_DSP_DIR_EINVAL;
    }
    ctx->ready = 0u;
    ctx->base = (uintptr_t)0;
    return TMS320C6452_DSP_DIR_OK;
}

int tms320c6452_dsp_direction_control_dir01_set_input(tms320c6452_dsp_dir_ctx_t *ctx, uint32_t pin)
{
    volatile uint32_t *dir_reg;
    uint32_t v;

    if (!dir01_check_ctx(ctx)) { return TMS320C6452_DSP_DIR_ESTATE; }
    if (!dir01_check_pin(pin)) { return TMS320C6452_DSP_DIR_EINVAL; }

    dir_reg = (volatile uint32_t *)((uintptr_t)ctx->base + (uint32_t)TMS320C6452_DSP_GPIO_DIR01_OFFSET);
    v = reg_read32_be(dir_reg);
    v |= TMS320C6452_DSP_GPIO_DIR_PIN_MASK(pin); /* 1 = input */
    reg_write32_be(dir_reg, v);
    return TMS320C6452_DSP_DIR_OK;
}

int tms320c6452_dsp_direction_control_dir01_set_output(tms320c6452_dsp_dir_ctx_t *ctx, uint32_t pin)
{
    volatile uint32_t *dir_reg;
    uint32_t v;

    if (!dir01_check_ctx(ctx)) { return TMS320C6452_DSP_DIR_ESTATE; }
    if (!dir01_check_pin(pin)) { return TMS320C6452_DSP_DIR_EINVAL; }

    dir_reg = (volatile uint32_t *)((uintptr_t)ctx->base + (uint32_t)TMS320C6452_DSP_GPIO_DIR01_OFFSET);
    v = reg_read32_be(dir_reg);
    v &= (uint32_t)~TMS320C6452_DSP_GPIO_DIR_PIN_MASK(pin); /* 0 = output */
    reg_write32_be(dir_reg, v);
    return TMS320C6452_DSP_DIR_OK;
}

int tms320c6452_dsp_direction_control_dir01_get(tms320c6452_dsp_dir_ctx_t *ctx, uint32_t pin, uint32_t *is_input)
{
    volatile uint32_t *dir_reg;
    uint32_t v;

    if (!dir01_check_ctx(ctx) || is_input == (uint32_t *)0) { return TMS320C6452_DSP_DIR_EINVAL; }
    if (!dir01_check_pin(pin)) { return TMS320C6452_DSP_DIR_EINVAL; }

    dir_reg = (volatile uint32_t *)((uintptr_t)ctx->base + (uint32_t)TMS320C6452_DSP_GPIO_DIR01_OFFSET);
    v = reg_read32_be(dir_reg);
    *is_input = (v >> pin) & 0x1u;
    return TMS320C6452_DSP_DIR_OK;
}

int tms320c6452_dsp_direction_control_dir01_write_mask(tms320c6452_dsp_dir_ctx_t *ctx, uint32_t dir_mask)
{
    volatile uint32_t *dir_reg;

    if (!dir01_check_ctx(ctx)) { return TMS320C6452_DSP_DIR_ESTATE; }

    dir_reg = (volatile uint32_t *)((uintptr_t)ctx->base + (uint32_t)TMS320C6452_DSP_GPIO_DIR01_OFFSET);
    reg_write32_be(dir_reg, dir_mask);
    return TMS320C6452_DSP_DIR_OK;
}

int tms320c6452_dsp_direction_control_dir01_read_mask(tms320c6452_dsp_dir_ctx_t *ctx, uint32_t *dir_mask)
{
    volatile uint32_t *dir_reg;

    if (!dir01_check_ctx(ctx) || dir_mask == (uint32_t *)0) { return TMS320C6452_DSP_DIR_EINVAL; }

    dir_reg = (volatile uint32_t *)((uintptr_t)ctx->base + (uint32_t)TMS320C6452_DSP_GPIO_DIR01_OFFSET);
    *dir_mask = reg_read32_be(dir_reg);
    return TMS320C6452_DSP_DIR_OK;
}
