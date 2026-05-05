/*
 * TMS320C6452 DSP GPIO Core Driver
 * Environment: ARM bare-metal, GCC, big-endian MMIO
 */
#include "TMS320C6452 DSP_core.h"

/* Volatile pointer helpers */
#define MMIO32(addr) (*(volatile uint32_t *)(addr))

/*
 * Big-endian MMIO helpers
 * For ARM big-endian targets with GCC, prefer explicit byte-swaps around
 * native little-endian loads/stores if CPU is little-endian at bus.
 * Here we assume memory space is big-endian mapped; adjust if needed.
 */
static uint32_t be32toh_inline(uint32_t x)
{
    /* No-op for big-endian target; if compiled LE, this should byteswap. */
#if (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
    return __builtin_bswap32(x);
#else
    return x;
#endif
}

static uint32_t htobe32_inline(uint32_t x)
{
#if (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
    return __builtin_bswap32(x);
#else
    return x;
#endif
}

uint32_t gpio_be_read32(uintptr_t addr)
{
    uint32_t v = MMIO32(addr);
    return be32toh_inline(v);
}

void gpio_be_write32(uintptr_t addr, uint32_t val)
{
    MMIO32(addr) = htobe32_inline(val);
}

/* Ops instance */
static const gpio_core_ops_t s_gpio_ops = {
    .reg_read32 = gpio_be_read32,
    .reg_write32 = gpio_be_write32,
    .pin_bit = gpio_pin_bit_calc,
    .bank_index = gpio_bank_index_calc,
};

const gpio_core_ops_t* gpio_core_get_ops(void)
{
    return &s_gpio_ops;
}
