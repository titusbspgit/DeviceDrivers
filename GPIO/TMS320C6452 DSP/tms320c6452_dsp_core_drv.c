#include "tms320c6452_dsp_core.h"

#ifndef TMS320C6452_DSP_MMIO_BIG_ENDIAN
#define TMS320C6452_DSP_MMIO_BIG_ENDIAN 1
#endif

static inline void core_mb(void)
{
    __asm__ __volatile__("" ::: "memory");
}

static inline uint32_t be32_to_cpu_core(uint32_t v)
{
#if TMS320C6452_DSP_MMIO_BIG_ENDIAN
  #if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
    return __builtin_bswap32(v);
  #else
    return v;
  #endif
#else
    return v;
#endif
}

static inline uint32_t cpu_to_be32_core(uint32_t v)
{
#if TMS320C6452_DSP_MMIO_BIG_ENDIAN
  #if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
    return __builtin_bswap32(v);
  #else
    return v;
  #endif
#else
    return v;
#endif
}

static uint32_t core_read32_impl(const tms320c6452_dsp_gpio_core_ctx_t* ctx, uintptr_t offset, uint32_t bank)
{
    volatile const uint32_t* p = (volatile const uint32_t*)(ctx->base + offset + (ctx->bank_stride * (uintptr_t)bank));
    uint32_t v = *p;
    core_mb();
    return be32_to_cpu_core(v);
}

static void core_write32_impl(const tms320c6452_dsp_gpio_core_ctx_t* ctx, uintptr_t offset, uint32_t bank, uint32_t value)
{
    core_mb();
    volatile uint32_t* p = (volatile uint32_t*)(ctx->base + offset + (ctx->bank_stride * (uintptr_t)bank));
    *p = cpu_to_be32_core(value);
    core_mb();
}

static inline bool is_unknown_u(const uintptr_t v)
{
    return v == TMS320C6452_DSP_CORE_REG_UNKNOWN;
}

static tms320c6452_dsp_core_status_t core_set_bits_impl(tms320c6452_dsp_gpio_core_ctx_t* ctx, uint32_t bank, uint32_t mask)
{
    if (is_unknown_u(ctx->bank_stride)) { return TMS320C6452_DSP_CORE_ERR_UNSUPPORTED; }

    if (!is_unknown_u(ctx->set_offset)) {
        core_write32_impl(ctx, ctx->set_offset, bank, mask);
        return TMS320C6452_DSP_CORE_OK;
    }
    if (!is_unknown_u(ctx->out_offset)) {
        uint32_t v = core_read32_impl(ctx, ctx->out_offset, bank);
        core_write32_impl(ctx, ctx->out_offset, bank, v | mask);
        return TMS320C6452_DSP_CORE_OK;
    }
    return TMS320C6452_DSP_CORE_ERR_UNSUPPORTED;
}

static tms320c6452_dsp_core_status_t core_clear_bits_impl(tms320c6452_dsp_gpio_core_ctx_t* ctx, uint32_t bank, uint32_t mask)
{
    if (is_unknown_u(ctx->bank_stride)) { return TMS320C6452_DSP_CORE_ERR_UNSUPPORTED; }

    if (!is_unknown_u(ctx->clr_offset)) {
        core_write32_impl(ctx, ctx->clr_offset, bank, mask);
        return TMS320C6452_DSP_CORE_OK;
    }
    if (!is_unknown_u(ctx->out_offset)) {
        uint32_t v = core_read32_impl(ctx, ctx->out_offset, bank);
        core_write32_impl(ctx, ctx->out_offset, bank, v & ~mask);
        return TMS320C6452_DSP_CORE_OK;
    }
    return TMS320C6452_DSP_CORE_ERR_UNSUPPORTED;
}

static tms320c6452_dsp_core_status_t core_read_bits_impl(tms320c6452_dsp_gpio_core_ctx_t* ctx, uint32_t bank, uint32_t* value)
{
    if ((value == NULL) || is_unknown_u(ctx->bank_stride)) { return TMS320C6452_DSP_CORE_ERR_UNSUPPORTED; }

    if (!is_unknown_u(ctx->in_offset)) {
        *value = core_read32_impl(ctx, ctx->in_offset, bank);
        return TMS320C6452_DSP_CORE_OK;
    }
    if (!is_unknown_u(ctx->out_offset)) {
        *value = core_read32_impl(ctx, ctx->out_offset, bank);
        return TMS320C6452_DSP_CORE_OK;
    }
    return TMS320C6452_DSP_CORE_ERR_UNSUPPORTED;
}

static tms320c6452_dsp_core_status_t core_config_dir_impl(tms320c6452_dsp_gpio_core_ctx_t* ctx, uint32_t bank, uint32_t mask, bool is_output)
{
    if (is_unknown_u(ctx->bank_stride) || is_unknown_u(ctx->dir_offset)) { return TMS320C6452_DSP_CORE_ERR_UNSUPPORTED; }
    uint32_t v = core_read32_impl(ctx, ctx->dir_offset, bank);
    v = is_output ? (v | mask) : (v & ~mask);
    core_write32_impl(ctx, ctx->dir_offset, bank, v);
    return TMS320C6452_DSP_CORE_OK;
}

static tms320c6452_dsp_core_status_t core_write_pin_impl(tms320c6452_dsp_gpio_core_ctx_t* ctx, uint32_t pin, bool value)
{
    uint32_t bank = TMS320C6452_DSP_GPIO_PIN_TO_BANK(pin);
    uint32_t bit  = TMS320C6452_DSP_GPIO_PIN_TO_BIT(pin);
    return value ? core_set_bits_impl(ctx, bank, TMS320C6452_DSP_GPIO_BIT(bit))
                 : core_clear_bits_impl(ctx, bank, TMS320C6452_DSP_GPIO_BIT(bit));
}

static tms320c6452_dsp_core_status_t core_read_pin_impl(tms320c6452_dsp_gpio_core_ctx_t* ctx, uint32_t pin, bool* value)
{
    if (value == NULL) { return TMS320C6452_DSP_CORE_ERR_PARAM; }
    uint32_t bank = TMS320C6452_DSP_GPIO_PIN_TO_BANK(pin);
    uint32_t bit  = TMS320C6452_DSP_GPIO_PIN_TO_BIT(pin);
    uint32_t v = 0u;
    tms320c6452_dsp_core_status_t st = core_read_bits_impl(ctx, bank, &v);
    if (st != TMS320C6452_DSP_CORE_OK) { return st; }
    *value = ((v & TMS320C6452_DSP_GPIO_BIT(bit)) != 0u);
    return TMS320C6452_DSP_CORE_OK;
}

static const tms320c6452_dsp_gpio_core_ops_t CORE_OPS_TEMPLATE = {
    .read32     = core_read32_impl,
    .write32    = core_write32_impl,
    .set_bits   = core_set_bits_impl,
    .clear_bits = core_clear_bits_impl,
    .read_bits  = core_read_bits_impl,
    .config_dir = core_config_dir_impl,
    .write_pin  = core_write_pin_impl,
    .read_pin   = core_read_pin_impl,
};

tms320c6452_dsp_core_status_t tms320c6452_dsp_gpio_core_init(
    tms320c6452_dsp_gpio_core_ctx_t* ctx,
    tms320c6452_dsp_gpio_core_ops_t* ops,
    uintptr_t base)
{
    if ((ctx == NULL) || (ops == NULL))
    {
        return TMS320C6452_DSP_CORE_ERR_PARAM;
    }

    ctx->base          = base;
    ctx->bank_stride   = TMS320C6452_DSP_CORE_REG_UNKNOWN;
    ctx->dir_offset    = TMS320C6452_DSP_CORE_REG_UNKNOWN;
    ctx->in_offset     = TMS320C6452_DSP_CORE_REG_UNKNOWN;
    ctx->out_offset    = TMS320C6452_DSP_CORE_REG_UNKNOWN;
    ctx->set_offset    = TMS320C6452_DSP_CORE_REG_UNKNOWN;
    ctx->clr_offset    = TMS320C6452_DSP_CORE_REG_UNKNOWN;
    ctx->int_en_offset = TMS320C6452_DSP_CORE_REG_UNKNOWN;
    ctx->int_sts_offset= TMS320C6452_DSP_CORE_REG_UNKNOWN;
    ctx->int_clr_offset= TMS320C6452_DSP_CORE_REG_UNKNOWN;

    *ops = CORE_OPS_TEMPLATE;
    return TMS320C6452_DSP_CORE_OK;
}
