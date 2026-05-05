/*
 * TMS320C6452 DSP GPIO Core Header
 * Environment: ARM bare-metal, GCC, big-endian MMIO
 * NOTE: Some register offsets are UNKNOWN and marked as TODO. Update with SoC TRM values.
 */
#ifndef TMS320C6452_DSP_CORE_H
#define TMS320C6452_DSP_CORE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Base address (from global configuration) */
#define TMS320C6452_GPIO_BASE   (0xA1008000u)

/* Register offsets (placeholders if UNKNOWN) */
#define GPIO_DIR01_OFFSET       (0x0000u) /* TODO: UNKNOWN */
#define GPIO_OUT_DATA01_OFFSET  (0x0004u) /* TODO: UNKNOWN */
#define GPIO_IN_DATA01_OFFSET   (0x0008u) /* TODO: UNKNOWN */
#define GPIO_SET_DATA01_OFFSET  (0x000Cu) /* TODO: UNKNOWN */
#define GPIO_CLR_DATA01_OFFSET  (0x0010u) /* TODO: UNKNOWN */
#define GPIO_BINTEN_OFFSET      (0x0014u) /* TODO: UNKNOWN */
#define GPIO_INTSTAT_OFFSET     (0x0018u) /* TODO: UNKNOWN */
#define GPIO_INTEN_OFFSET       (0x001Cu) /* TODO: UNKNOWN */
#define GPIO_INTEDGE_OFFSET     (0x0020u) /* TODO: UNKNOWN */
#define GPIO_INTCLR_OFFSET      (0x0024u) /* TODO: UNKNOWN */

/* Convenience absolute addresses */
#define GPIO_REG_ADDR(ofs)      (TMS320C6452_GPIO_BASE + (ofs))
#define GPIO_DIR01_ADDR         GPIO_REG_ADDR(GPIO_DIR01_OFFSET)
#define GPIO_OUT_DATA01_ADDR    GPIO_REG_ADDR(GPIO_OUT_DATA01_OFFSET)
#define GPIO_IN_DATA01_ADDR     GPIO_REG_ADDR(GPIO_IN_DATA01_OFFSET)
#define GPIO_SET_DATA01_ADDR    GPIO_REG_ADDR(GPIO_SET_DATA01_OFFSET)
#define GPIO_CLR_DATA01_ADDR    GPIO_REG_ADDR(GPIO_CLR_DATA01_OFFSET)
#define GPIO_BINTEN_ADDR        GPIO_REG_ADDR(GPIO_BINTEN_OFFSET)
#define GPIO_INTSTAT_ADDR       GPIO_REG_ADDR(GPIO_INTSTAT_OFFSET)
#define GPIO_INTEN_ADDR         GPIO_REG_ADDR(GPIO_INTEN_OFFSET)
#define GPIO_INTEDGE_ADDR       GPIO_REG_ADDR(GPIO_INTEDGE_OFFSET)
#define GPIO_INTCLR_ADDR        GPIO_REG_ADDR(GPIO_INTCLR_OFFSET)

/* Bit utilities */
#define GPIO_PIN_MAX            (32u)
#define GPIO_PINS_PER_BANK      (16u)

/* Inline-safe, MISRA-friendly types */
#include <stdint.h>
#include <stddef.h>

/* Core ops structure for reuse by feature drivers */
typedef struct gpio_core_ops_tag {
    uint32_t (*reg_read32)(uintptr_t addr);
    void     (*reg_write32)(uintptr_t addr, uint32_t val);
    uint32_t (*pin_bit)(uint32_t pin_index);
    uint32_t (*bank_index)(uint32_t pin_index);
} gpio_core_ops_t;

/* Big-endian 32-bit MMIO accessors (declarations) */
uint32_t gpio_be_read32(uintptr_t addr);
void     gpio_be_write32(uintptr_t addr, uint32_t val);

/* Helpers */
static inline uint32_t gpio_pin_bit_calc(uint32_t pin_index)
{
    /* pin 0..31 -> bit within 32-bit register */
    return (uint32_t)(1u << (pin_index & 0x1Fu));
}

static inline uint32_t gpio_bank_index_calc(uint32_t pin_index)
{
    /* two banks of 16 */
    return (uint32_t)(pin_index >> 4); /* 0 for 0..15, 1 for 16..31 */
}

/* Exported ops instance getter */
const gpio_core_ops_t* gpio_core_get_ops(void);

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_DSP_CORE_H */
