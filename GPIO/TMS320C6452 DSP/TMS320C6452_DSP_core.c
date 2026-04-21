/*
 * TMS320C6452 DSP GPIO Core - implementation
 * NOTE: Register offsets are scaffolds (see header). Replace with TRM-verified offsets.
 */
#include "TMS320C6452_DSP_core.h"

/* Optional: if building under U-Boot and you prefer in_be32/out_be32, define GPIO_CORE_USE_UBOOT_IO */
#ifdef GPIO_CORE_USE_UBOOT_IO
#include <asm/io.h>
#endif

#ifndef __maybe_unused
#define __maybe_unused __attribute__((unused))
#endif

/* Local byteswap helper (portable) */
static inline uint32_t _bswap32(uint32_t v)
{
#if defined(__GNUC__)
    return __builtin_bswap32(v);
#else
    return ((v & 0x000000FFu) << 24) |
           ((v & 0x0000FF00u) << 8)  |
           ((v & 0x00FF0000u) >> 8)  |
           ((v & 0xFF000000u) >> 24);
#endif
}

/* MMIO accessors with endianness handling */
static inline uint32_t _mmio_read32(gpio_core_t *ctx, uintptr_t off)
{
    volatile uint32_t *p = (volatile uint32_t *)((uintptr_t)ctx->base + off);
#ifdef GPIO_CORE_USE_UBOOT_IO
    if (ctx->big_endian_regs) {
#ifdef in_be32
        return in_be32(p);
#else
        return _bswap32(*p); /* Fallback if in_be32 not available */
#endif
    } else {
#ifdef in_le32
        return in_le32(p);
#else
        return *p; /* Assume CPU-endian == LE */
#endif
    }
#else
    /* Generic fallback: treat memory as CPU-endian and swap if registers are big-endian */
    uint32_t v = *p;
    return ctx->big_endian_regs ? _bswap32(v) : v;
#endif
}

static inline void _mmio_write32(gpio_core_t *ctx, uintptr_t off, uint32_t val)
{
    volatile uint32_t *p = (volatile uint32_t *)((uintptr_t)ctx->base + off);
#ifdef GPIO_CORE_USE_UBOOT_IO
    if (ctx->big_endian_regs) {
#ifdef out_be32
        out_be32(p, val);
#else
        *p = _bswap32(val); /* Fallback if out_be32 not available */
#endif
    } else {
#ifdef out_le32
        out_le32(p, val);
#else
        *p = val; /* Assume CPU-endian == LE */
#endif
    }
#else
    *p = ctx->big_endian_regs ? _bswap32(val) : val;
#endif
}

/* Bounds check */
static inline bool _valid_pin(unsigned pin) { return pin < GPIO_MAX_PINS; }
static inline bool _valid_bank(unsigned bank) { return bank < GPIO_NUM_BANKS; }

/* Core ops implementations */
static uint32_t _api_reg_read32(gpio_core_t *ctx, uintptr_t off)
{
    return _mmio_read32(ctx, off);
}

static void _api_reg_write32(gpio_core_t *ctx, uintptr_t off, uint32_t val)
{
    _mmio_write32(ctx, off, val);
}

static int _api_set_dir(gpio_core_t *ctx, unsigned pin, bool is_input)
{
    if (!_valid_pin(pin)) return -1;
    uint32_t dir = _mmio_read32(ctx, GPIO_DIR01_OFFSET);
    uint32_t bit = (uint32_t)1u << pin;
    if (is_input) dir |= bit; else dir &= ~bit;
    _mmio_write32(ctx, GPIO_DIR01_OFFSET, dir);
    return 0;
}

static int _api_get_dir(gpio_core_t *ctx, unsigned pin, bool *is_input)
{
    if (!_valid_pin(pin) || !is_input) return -1;
    uint32_t dir = _mmio_read32(ctx, GPIO_DIR01_OFFSET);
    *is_input = ((dir >> pin) & 1u) ? true : false;
    return 0;
}

static int _api_write_pin(gpio_core_t *ctx, unsigned pin, bool value)
{
    if (!_valid_pin(pin)) return -1;
    uint32_t bit = (uint32_t)1u << pin;
    if (value)
        _mmio_write32(ctx, GPIO_SET_DATA01_OFFSET, bit);
    else
        _mmio_write32(ctx, GPIO_CLR_DATA01_OFFSET, bit);
    return 0;
}

static int _api_read_pin(gpio_core_t *ctx, unsigned pin, bool *value)
{
    if (!_valid_pin(pin) || !value) return -1;
    uint32_t in = _mmio_read32(ctx, GPIO_IN_DATA01_OFFSET);
    *value = ((in >> pin) & 1u) ? true : false;
    return 0;
}

static int _api_set_bits(gpio_core_t *ctx, uint32_t mask)
{
    _mmio_write32(ctx, GPIO_SET_DATA01_OFFSET, mask);
    return 0;
}

static int _api_clear_bits(gpio_core_t *ctx, uint32_t mask)
{
    _mmio_write32(ctx, GPIO_CLR_DATA01_OFFSET, mask);
    return 0;
}

static int _api_toggle_bits(gpio_core_t *ctx, uint32_t mask)
{
    /* Read current, compute new, write via SET/CLR to avoid read-modify-write races */
    uint32_t cur = _mmio_read32(ctx, GPIO_OUT_DATA01_OFFSET);
    uint32_t newv = cur ^ mask;
    uint32_t setm = (~cur) & newv; /* bits going 0->1 */
    uint32_t clrm = cur & (~newv); /* bits going 1->0 */
    if (setm) _mmio_write32(ctx, GPIO_SET_DATA01_OFFSET, setm);
    if (clrm) _mmio_write32(ctx, GPIO_CLR_DATA01_OFFSET, clrm);
    return 0;
}

static int _api_enable_bank_irq(gpio_core_t *ctx, unsigned bank, bool enable)
{
    if (!_valid_bank(bank)) return -1;
    uint32_t v = _mmio_read32(ctx, GPIO_BINTEN_OFFSET);
    uint32_t bit = (uint32_t)1u << bank;
    if (enable) v |= bit; else v &= ~bit;
    _mmio_write32(ctx, GPIO_BINTEN_OFFSET, v);
    return 0;
}

/* Static function table */
static const gpio_core_api_t _api = {
    .reg_read32    = _api_reg_read32,
    .reg_write32   = _api_reg_write32,
    .set_dir       = _api_set_dir,
    .get_dir       = _api_get_dir,
    .write_pin     = _api_write_pin,
    .read_pin      = _api_read_pin,
    .set_bits      = _api_set_bits,
    .clear_bits    = _api_clear_bits,
    .toggle_bits   = _api_toggle_bits,
    .enable_bank_irq = _api_enable_bank_irq,
};

const gpio_core_api_t *gpio_core_init(gpio_core_t *ctx, void *base, bool big_endian_regs)
{
    if (!ctx) return NULL;
    ctx->base = (volatile uint8_t *)(base ? base : (void *)TMS320C6452_DSP_GPIO_BASE);
    ctx->big_endian_regs = big_endian_regs;
    return &_api;
}
