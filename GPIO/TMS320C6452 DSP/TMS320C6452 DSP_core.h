#ifndef TMS320C6452_DSP_CORE_H
#define TMS320C6452_DSP_CORE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Assumptions:
 * - Memory-mapped IO
 * - Big-endian system accesses
 * - 32-bit aligned register accesses
 * - Baremetal, no OS services
 */

#define TMS320C6452_GPIO_BASE_ADDR   (0xA1008000u)

/* Representative GPIO-style register offsets (subset) */
#define GPIO_DIR_OFFSET          (0x10u)
#define GPIO_OUT_OFFSET          (0x14u)
#define GPIO_SET_OFFSET          (0x18u)
#define GPIO_CLR_OFFSET          (0x1Cu)
#define GPIO_IN_OFFSET           (0x20u)
#define GPIO_BINTEN_OFFSET       (0x08u)
#define GPIO_SET_RIS_TRIG_OFFSET (0x24u)
#define GPIO_CLR_RIS_TRIG_OFFSET (0x28u)
#define GPIO_SET_FAL_TRIG_OFFSET (0x2Cu)
#define GPIO_CLR_FAL_TRIG_OFFSET (0x30u)
#define GPIO_INTSTAT_OFFSET      (0x34u)

struct gpio_core_ops {
    uint32_t (*read32)(uintptr_t addr);
    void     (*write32)(uintptr_t addr, uint32_t val);
    void     (*bitset32)(uintptr_t addr, uint32_t mask);
    void     (*bitclear32)(uintptr_t addr, uint32_t mask);
    uintptr_t base;
    bool is_big_endian;
};

static inline uint32_t gpio_swap32_be(uint32_t v)
{
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    return __builtin_bswap32(v);
#else
    return v;
#endif
}

static inline uint32_t gpio_reg_read32(uintptr_t addr)
{
    volatile const uint32_t* p = (volatile const uint32_t*)addr;
    uint32_t v = *p;
    return gpio_swap32_be(v);
}

static inline void gpio_reg_write32(uintptr_t addr, uint32_t val)
{
    volatile uint32_t* p = (volatile uint32_t*)addr;
    *p = gpio_swap32_be(val);
}

static inline void gpio_bitset32(uintptr_t addr, uint32_t mask)
{
    uint32_t v = gpio_reg_read32(addr);
    v |= mask;
    gpio_reg_write32(addr, v);
}

static inline void gpio_bitclear32(uintptr_t addr, uint32_t mask)
{
    uint32_t v = gpio_reg_read32(addr);
    v &= ~mask;
    gpio_reg_write32(addr, v);
}

/* Provide a default static ops implementation referencing the base address */
static const struct gpio_core_ops g_gpio_core_ops = {
    .read32 = gpio_reg_read32,
    .write32 = gpio_reg_write32,
    .bitset32 = gpio_bitset32,
    .bitclear32 = gpio_bitclear32,
    .base = (uintptr_t)TMS320C6452_GPIO_BASE_ADDR,
    .is_big_endian = true
};

static inline const struct gpio_core_ops* gpio_core_get_ops(void)
{
    return &g_gpio_core_ops;
}

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_DSP_CORE_H */
