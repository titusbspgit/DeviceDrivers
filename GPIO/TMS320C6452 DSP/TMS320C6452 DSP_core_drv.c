/*
 * TMS320C6452 DSP GPIO Core Driver - Source
 * Provides big-endian aware MMIO helpers and core ops for GPIO-like registers.
 * Assumptions: memory-mapped IO, 32-bit registers, volatile, aligned accesses.
 */
#include "TMS320C6452 DSP_core.h"

/* Endianness utilities: handle device big-endian vs host endianness. */
static inline uint32_t be32_to_cpu(uint32_t x)
{
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    return __builtin_bswap32(x);
#else
    return x;
#endif
}

static inline uint32_t cpu_to_be32(uint32_t x)
{
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    return __builtin_bswap32(x);
#else
    return x;
#endif
}

/* Volatile pointer accessor */
static inline volatile uint32_t* reg_ptr32(uintptr_t addr)
{
    return (volatile uint32_t*)(addr);
}

uint32_t gpio_reg_read32(uintptr_t addr)
{
    /* Device registers are big-endian */
    const volatile uint32_t raw = *reg_ptr32(addr);
    return be32_to_cpu(raw);
}

void gpio_reg_write32(uintptr_t addr, uint32_t val)
{
    /* Convert from CPU endianness to device big-endian before write */
    *reg_ptr32(addr) = cpu_to_be32(val);
}

void gpio_reg_set_bits(uintptr_t addr, uint32_t mask)
{
    uint32_t v = gpio_reg_read32(addr);
    v |= mask;
    gpio_reg_write32(addr, v);
}

void gpio_reg_clear_bits(uintptr_t addr, uint32_t mask)
{
    uint32_t v = gpio_reg_read32(addr);
    v &= (uint32_t)(~mask);
    gpio_reg_write32(addr, v);
}

void gpio_reg_update_bits(uintptr_t addr, uint32_t mask, uint32_t value)
{
    uint32_t v = gpio_reg_read32(addr);
    v = (uint32_t)((v & (~mask)) | (value & mask));
    gpio_reg_write32(addr, v);
}

/* Static ops instance */
static const struct gpio_core_ops gpio_core = {
    .read32       = gpio_reg_read32,
    .write32      = gpio_reg_write32,
    .set_bits     = gpio_reg_set_bits,
    .clear_bits   = gpio_reg_clear_bits,
    .update_bits  = gpio_reg_update_bits,
    .base         = TMS320C6452_DSP_GPIO_BASE,
    .device_is_be = (uint8_t)1,
};

const struct gpio_core_ops* gpio_core_get_ops(void)
{
    return &gpio_core;
}
