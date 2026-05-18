#include "tms320c6452_dsp_gpio_banked_architecture__32_signals__2_banks.h"

#ifndef TMS320C6452_DSP_MMIO_BIG_ENDIAN
#define TMS320C6452_DSP_MMIO_BIG_ENDIAN 1
#endif

static inline void tms320c6452_dsp_mmio_mb(void)
{
    __asm__ __volatile__ ("" ::: "memory");
}

static inline uint32_t tms320c6452_dsp_be32_to_cpu(uint32_t v)
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

static inline uint32_t tms320c6452_dsp_cpu_to_be32(uint32_t v)
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

static inline uint32_t tms320c6452_dsp_mmio_read32(uintptr_t addr)
{
    volatile const uint32_t* p = (volatile const uint32_t*)addr;
    uint32_t v = *p;
    tms320c6452_dsp_mmio_mb();
    return tms320c6452_dsp_be32_to_cpu(v);
}

static inline void tms320c6452_dsp_mmio_write32(uintptr_t addr, uint32_t val)
{
    tms320c6452_dsp_mmio_mb();
    volatile uint32_t* p = (volatile uint32_t*)addr;
    *p = tms320c6452_dsp_cpu_to_be32(val);
    tms320c6452_dsp_mmio_mb();
}

static inline bool tms320c6452_dsp_is_unknown(uintptr_t off)
{
    return (off == TMS320C6452_DSP_GPIO_REG_UNKNOWN);
}

static inline bool tms320c6452_dsp_valid_bank(uint32_t bank)
{
    return (bank < TMS320C6452_DSP_GPIO_BANK_COUNT);
}

static inline bool tms320c6452_dsp_valid_pin(uint32_t pin)
{
    return (pin < TMS320C6452_DSP_GPIO_TOTAL_PINS);
}

static inline uintptr_t tms320c6452_dsp_reg_addr(const tms320c6452_dsp_gpio_banked_ctx_t* ctx, uintptr_t off, uint32_t bank)
{
    return (ctx->cfg.base + off + (ctx->cfg.bank_stride * (uintptr_t)bank));
}

tms320c6452_dsp_gpio_status_t
tms320c6452_dsp_gpio_banked_init(tms320c6452_dsp_gpio_banked_ctx_t* ctx,
                                 const tms320c6452_dsp_gpio_banked_cfg_t* cfg)
{
    if ((ctx == NULL) || (cfg == NULL))
    {
        return TMS320C6452_DSP_GPIO_ERR_PARAM;
    }
    ctx->cfg = *cfg;
    return TMS320C6452_DSP_GPIO_OK;
}

tms320c6452_dsp_gpio_status_t
tms320c6452_dsp_gpio_banked_config_dir_bank(tms320c6452_dsp_gpio_banked_ctx_t* ctx,
                                            uint32_t bank,
                                            uint32_t mask,
                                            bool is_output)
{
    if ((ctx == NULL) || !tms320c6452_dsp_valid_bank(bank))
    {
        return TMS320C6452_DSP_GPIO_ERR_PARAM;
    }
    if (tms320c6452_dsp_is_unknown(ctx->cfg.bank_stride) || tms320c6452_dsp_is_unknown(ctx->cfg.dir_offset))
    {
        return TMS320C6452_DSP_GPIO_ERR_UNSUPPORTED;
    }

    uintptr_t a = tms320c6452_dsp_reg_addr(ctx, ctx->cfg.dir_offset, bank);
    uint32_t v = tms320c6452_dsp_mmio_read32(a);
    if (is_output)
    {
        v |= mask;
    }
    else
    {
        v &= ~mask;
    }
    tms320c6452_dsp_mmio_write32(a, v);
    return TMS320C6452_DSP_GPIO_OK;
}

tms320c6452_dsp_gpio_status_t
tms320c6452_dsp_gpio_banked_write_bank_mask(tms320c6452_dsp_gpio_banked_ctx_t* ctx,
                                            uint32_t bank,
                                            uint32_t set_mask,
                                            uint32_t clear_mask)
{
    if ((ctx == NULL) || !tms320c6452_dsp_valid_bank(bank))
    {
        return TMS320C6452_DSP_GPIO_ERR_PARAM;
    }
    if (tms320c6452_dsp_is_unknown(ctx->cfg.bank_stride))
    {
        return TMS320C6452_DSP_GPIO_ERR_UNSUPPORTED;
    }

    if (!tms320c6452_dsp_is_unknown(ctx->cfg.set_offset))
    {
        uintptr_t a_set = tms320c6452_dsp_reg_addr(ctx, ctx->cfg.set_offset, bank);
        tms320c6452_dsp_mmio_write32(a_set, set_mask);
    }
    else if (!tms320c6452_dsp_is_unknown(ctx->cfg.out_offset))
    {
        uintptr_t a_out = tms320c6452_dsp_reg_addr(ctx, ctx->cfg.out_offset, bank);
        uint32_t v = tms320c6452_dsp_mmio_read32(a_out);
        v |= set_mask;
        tms320c6452_dsp_mmio_write32(a_out, v);
    }
    else
    {
        return TMS320C6452_DSP_GPIO_ERR_UNSUPPORTED;
    }

    if (!tms320c6452_dsp_is_unknown(ctx->cfg.clr_offset))
    {
        uintptr_t a_clr = tms320c6452_dsp_reg_addr(ctx, ctx->cfg.clr_offset, bank);
        tms320c6452_dsp_mmio_write32(a_clr, clear_mask);
    }
    else if (!tms320c6452_dsp_is_unknown(ctx->cfg.out_offset))
    {
        uintptr_t a_out = tms320c6452_dsp_reg_addr(ctx, ctx->cfg.out_offset, bank);
        uint32_t v = tms320c6452_dsp_mmio_read32(a_out);
        v &= ~clear_mask;
        tms320c6452_dsp_mmio_write32(a_out, v);
    }
    else
    {
        return TMS320C6452_DSP_GPIO_ERR_UNSUPPORTED;
    }

    return TMS320C6452_DSP_GPIO_OK;
}

tms320c6452_dsp_gpio_status_t
tms320c6452_dsp_gpio_banked_read_bank(tms320c6452_dsp_gpio_banked_ctx_t* ctx,
                                      uint32_t bank,
                                      uint32_t* mask)
{
    if ((ctx == NULL) || (mask == NULL) || !tms320c6452_dsp_valid_bank(bank))
    {
        return TMS320C6452_DSP_GPIO_ERR_PARAM;
    }
    if (tms320c6452_dsp_is_unknown(ctx->cfg.bank_stride))
    {
        return TMS320C6452_DSP_GPIO_ERR_UNSUPPORTED;
    }

    if (!tms320c6452_dsp_is_unknown(ctx->cfg.in_offset))
    {
        uintptr_t a_in = tms320c6452_dsp_reg_addr(ctx, ctx->cfg.in_offset, bank);
        *mask = tms320c6452_dsp_mmio_read32(a_in);
        return TMS320C6452_DSP_GPIO_OK;
    }
    else if (!tms320c6452_dsp_is_unknown(ctx->cfg.out_offset))
    {
        uintptr_t a_out = tms320c6452_dsp_reg_addr(ctx, ctx->cfg.out_offset, bank);
        *mask = tms320c6452_dsp_mmio_read32(a_out);
        return TMS320C6452_DSP_GPIO_OK;
    }
    else
    {
        return TMS320C6452_DSP_GPIO_ERR_UNSUPPORTED;
    }
}

tms320c6452_dsp_gpio_status_t
tms320c6452_dsp_gpio_banked_write_pin(tms320c6452_dsp_gpio_banked_ctx_t* ctx,
                                      uint32_t pin,
                                      bool value)
{
    if ((ctx == NULL) || !tms320c6452_dsp_valid_pin(pin))
    {
        return TMS320C6452_DSP_GPIO_ERR_PARAM;
    }
    uint32_t bank = TMS320C6452_DSP_GPIO_PIN_TO_BANK(pin);
    uint32_t bit  = TMS320C6452_DSP_GPIO_PIN_TO_BIT(pin);
    uint32_t set_mask = value ? TMS320C6452_DSP_GPIO_BIT(bit) : 0u;
    uint32_t clr_mask = value ? 0u : TMS320C6452_DSP_GPIO_BIT(bit);
    return tms320c6452_dsp_gpio_banked_write_bank_mask(ctx, bank, set_mask, clr_mask);
}

tms320c6452_dsp_gpio_status_t
tms320c6452_dsp_gpio_banked_read_pin(tms320c6452_dsp_gpio_banked_ctx_t* ctx,
                                     uint32_t pin,
                                     bool* value)
{
    if ((ctx == NULL) || (value == NULL) || !tms320c6452_dsp_valid_pin(pin))
    {
        return TMS320C6452_DSP_GPIO_ERR_PARAM;
    }
    uint32_t bank = TMS320C6452_DSP_GPIO_PIN_TO_BANK(pin);
    uint32_t bit  = TMS320C6452_DSP_GPIO_PIN_TO_BIT(pin);
    uint32_t mask = 0u;
    tms320c6452_dsp_gpio_status_t st = tms320c6452_dsp_gpio_banked_read_bank(ctx, bank, &mask);
    if (st != TMS320C6452_DSP_GPIO_OK)
    {
        return st;
    }
    *value = ((mask & TMS320C6452_DSP_GPIO_BIT(bit)) != 0u);
    return TMS320C6452_DSP_GPIO_OK;
}

tms320c6452_dsp_gpio_status_t
tms320c6452_dsp_gpio_banked_irq_enable_bank(tms320c6452_dsp_gpio_banked_ctx_t* ctx,
                                            uint32_t bank,
                                            uint32_t enable_mask)
{
    if ((ctx == NULL) || !tms320c6452_dsp_valid_bank(bank))
    {
        return TMS320C6452_DSP_GPIO_ERR_PARAM;
    }
    if (tms320c6452_dsp_is_unknown(ctx->cfg.bank_stride) || tms320c6452_dsp_is_unknown(ctx->cfg.int_en_offset))
    {
        return TMS320C6452_DSP_GPIO_ERR_UNSUPPORTED;
    }

    uintptr_t a = tms320c6452_dsp_reg_addr(ctx, ctx->cfg.int_en_offset, bank);
    uint32_t v = tms320c6452_dsp_mmio_read32(a);
    v |= enable_mask;
    tms320c6452_dsp_mmio_write32(a, v);
    return TMS320C6452_DSP_GPIO_OK;
}

tms320c6452_dsp_gpio_status_t
tms320c6452_dsp_gpio_banked_irq_clear_bank(tms320c6452_dsp_gpio_banked_ctx_t* ctx,
                                           uint32_t bank,
                                           uint32_t clear_mask)
{
    if ((ctx == NULL) || !tms320c6452_dsp_valid_bank(bank))
    {
        return TMS320C6452_DSP_GPIO_ERR_PARAM;
    }
    if (tms320c6452_dsp_is_unknown(ctx->cfg.bank_stride))
    {
        return TMS320C6452_DSP_GPIO_ERR_UNSUPPORTED;
    }

    if (!tms320c6452_dsp_is_unknown(ctx->cfg.int_clr_offset))
    {
        uintptr_t a = tms320c6452_dsp_reg_addr(ctx, ctx->cfg.int_clr_offset, bank);
        tms320c6452_dsp_mmio_write32(a, clear_mask);
        return TMS320C6452_DSP_GPIO_OK;
    }
    else if (!tms320c6452_dsp_is_unknown(ctx->cfg.int_sts_offset))
    {
        uintptr_t a = tms320c6452_dsp_reg_addr(ctx, ctx->cfg.int_sts_offset, bank);
        uint32_t v = tms320c6452_dsp_mmio_read32(a);
        v &= ~clear_mask;
        tms320c6452_dsp_mmio_write32(a, v);
        return TMS320C6452_DSP_GPIO_OK;
    }
    else
    {
        return TMS320C6452_DSP_GPIO_ERR_UNSUPPORTED;
    }
}
