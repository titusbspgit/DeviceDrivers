/* Core GPIO driver source for TMS320C6452 DSP
 * Environment: ARM, Baremetal, GCC
 * Endianness handling: Registers are big-endian. Helpers swap on little-endian CPUs.
 */
#include "TMS320C6452 DSP_core.h"

/* Compile-time endianness detection (GCC/Clang) */
#if (defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)) || \
    defined(__ARMEB__) || defined(__BIG_ENDIAN__)
#define GPIO_CORE_CPU_BIG_ENDIAN   (1)
#else
#define GPIO_CORE_CPU_BIG_ENDIAN   (0)
#endif

/* Local byte-swap utility (GCC builtin) */
static inline uint32_t gpio_core_bswap32(uint32_t v)
{
#if defined(__has_builtin)
  #if __has_builtin(__builtin_bswap32)
    return __builtin_bswap32(v);
  #else
    return (uint32_t)(((v & 0x000000FFu) << 24) |
                      ((v & 0x0000FF00u) << 8)  |
                      ((v & 0x00FF0000u) >> 8)  |
                      ((v & 0xFF000000u) >> 24));
  #endif
#else
    return (uint32_t)(((v & 0x000000FFu) << 24) |
                      ((v & 0x0000FF00u) << 8)  |
                      ((v & 0x00FF0000u) >> 8)  |
                      ((v & 0xFF000000u) >> 24));
#endif
}

/* MMIO 32-bit read with endianness handling */
uint32_t gpio_core_reg_read32(uintptr_t addr)
{
    volatile const uint32_t *p = (volatile const uint32_t *)addr;
    uint32_t v = *p; /* deterministic 32-bit access */
#if GPIO_CORE_CPU_BIG_ENDIAN
    return v;
#else
    return gpio_core_bswap32(v);
#endif
}

/* MMIO 32-bit write with endianness handling */
void gpio_core_reg_write32(uintptr_t addr, uint32_t value)
{
#if GPIO_CORE_CPU_BIG_ENDIAN
    volatile uint32_t *p = (volatile uint32_t *)addr;
    *p = value;
#else
    volatile uint32_t *p = (volatile uint32_t *)addr;
    *p = gpio_core_bswap32(value);
#endif
    (void)addr; /* MISRA: addr used via p */
}

/* Pin mapping helpers for 32 pins in two banks of 16 */
uint32_t gpio_core_pin_to_bank(uint32_t pin)
{
    /* Caller must ensure pin < GPIO_NUM_PINS */
    return (uint32_t)(pin >> 4); /* divide by 16 -> bank 0 or 1 */
}

uint32_t gpio_core_pin_to_bit(uint32_t pin)
{
    return (uint32_t)(pin & (GPIO_BANK_WIDTH - 1u)); /* 0..15 */
}

uint32_t gpio_core_pin_to_mask32(uint32_t pin)
{
    return (uint32_t)(1u << (pin & 31u));
}

/* Immutable core ops instance */
static const gpio_core_ops_t s_gpio_core_ops = {
    .base           = GPIO_BASE_ADDR,
    .reg_read32     = gpio_core_reg_read32,
    .reg_write32    = gpio_core_reg_write32,
    .pin_to_bank    = gpio_core_pin_to_bank,
    .pin_to_bit     = gpio_core_pin_to_bit,
    .pin_to_mask32  = gpio_core_pin_to_mask32,
};

const gpio_core_ops_t* tms320c6452_dsp_gpio_core_get_ops(void)
{
    return &s_gpio_core_ops;
}
