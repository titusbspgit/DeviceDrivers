/* Core GPIO driver header for TMS320C6452 DSP
 * Environment: ARM, Baremetal, GCC, Big-endian MMIO
 * Notes: Register offsets are placeholders marked UNKNOWN. Verify with silicon docs.
 */
#ifndef TMS320C6452_DSP_CORE_H
#define TMS320C6452_DSP_CORE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

/* Base address */
#define GPIO_BASE_ADDR                 (0xA1008000u)

/* GPIO architecture constants */
#define GPIO_NUM_PINS                  (32u)
#define GPIO_NUM_BANKS                 (2u)
#define GPIO_BANK_WIDTH                (16u)

/* Register offsets (TODO: UNKNOWN values; placeholders for integration) */
#define GPIO_DIR01_OFFSET              (0x0000u) /* TODO: UNKNOWN */
#define GPIO_OUT_DATA01_OFFSET         (0x0004u) /* TODO: UNKNOWN */
#define GPIO_IN_DATA01_OFFSET          (0x0008u) /* TODO: UNKNOWN */
#define GPIO_SET_DATA01_OFFSET         (0x000Cu) /* TODO: UNKNOWN */
#define GPIO_CLR_DATA01_OFFSET         (0x0010u) /* TODO: UNKNOWN */
#define GPIO_BINTEN_OFFSET             (0x0014u) /* TODO: UNKNOWN */
#define GPIO_INTSTAT_OFFSET            (0x0018u) /* TODO: UNKNOWN */
#define GPIO_INTEN_OFFSET              (0x001Cu) /* TODO: UNKNOWN */
#define GPIO_INTEDGE_OFFSET            (0x0020u) /* TODO: UNKNOWN */
#define GPIO_INTCLR_OFFSET             (0x0024u) /* TODO: UNKNOWN */

/* Register absolute addresses (computed from base + offset) */
#define GPIO_DIR01_ADDR                (GPIO_BASE_ADDR + GPIO_DIR01_OFFSET)
#define GPIO_OUT_DATA01_ADDR           (GPIO_BASE_ADDR + GPIO_OUT_DATA01_OFFSET)
#define GPIO_IN_DATA01_ADDR            (GPIO_BASE_ADDR + GPIO_IN_DATA01_OFFSET)
#define GPIO_SET_DATA01_ADDR           (GPIO_BASE_ADDR + GPIO_SET_DATA01_OFFSET)
#define GPIO_CLR_DATA01_ADDR           (GPIO_BASE_ADDR + GPIO_CLR_DATA01_OFFSET)
#define GPIO_BINTEN_ADDR               (GPIO_BASE_ADDR + GPIO_BINTEN_OFFSET)
#define GPIO_INTSTAT_ADDR              (GPIO_BASE_ADDR + GPIO_INTSTAT_OFFSET)
#define GPIO_INTEN_ADDR                (GPIO_BASE_ADDR + GPIO_INTEN_OFFSET)
#define GPIO_INTEDGE_ADDR              (GPIO_BASE_ADDR + GPIO_INTEDGE_OFFSET)
#define GPIO_INTCLR_ADDR               (GPIO_BASE_ADDR + GPIO_INTCLR_OFFSET)

/* Bitfield helpers for 32-pin, two-bank layout */
#define GPIO_PIN_MAX_INDEX             (GPIO_NUM_PINS - 1u)

/* Core ops structure providing reusable helpers */
typedef struct gpio_core_ops_s {
    uintptr_t base; /* Base MMIO address */
    uint32_t (*reg_read32)(uintptr_t addr);
    void     (*reg_write32)(uintptr_t addr, uint32_t value);
    uint32_t (*pin_to_bank)(uint32_t pin);   /* 0..1 */
    uint32_t (*pin_to_bit)(uint32_t pin);    /* 0..15 */
    uint32_t (*pin_to_mask32)(uint32_t pin); /* 1u << pin */
} gpio_core_ops_t;

/* MMIO accessors (big-endian aware) */
uint32_t gpio_core_reg_read32(uintptr_t addr);
void     gpio_core_reg_write32(uintptr_t addr, uint32_t value);

/* Pin mapping helpers */
uint32_t gpio_core_pin_to_bank(uint32_t pin);
uint32_t gpio_core_pin_to_bit(uint32_t pin);
uint32_t gpio_core_pin_to_mask32(uint32_t pin);

/* Obtain immutable core ops for TMS320C6452 DSP */
const gpio_core_ops_t* tms320c6452_dsp_gpio_core_get_ops(void);

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_DSP_CORE_H */
