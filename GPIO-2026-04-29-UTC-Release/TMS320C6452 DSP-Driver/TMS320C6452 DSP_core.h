/*
 * TMS320C6452 DSP GPIO Core Driver - Header
 * Assumptions:
 * - Memory-mapped IO peripheral at base 0xA1008000
 * - Device register endianness: big-endian (32-bit registers)
 * - Volatile accesses; 32-bit aligned; C99; no dynamic memory
 */
#ifndef TMS320C6452_DSP_CORE_H
#define TMS320C6452_DSP_CORE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Base address for GPIO register block */
#define TMS320C6452_DSP_GPIO_BASE   ((uintptr_t)0xA1008000u)

/* Register offsets (representative GPIO-style) */
#define GPIO_DIR_OFFSET             (0x0000u) /* Direction: 1=output, 0=input */
#define GPIO_OUT_OFFSET             (0x0004u) /* Output data register */
#define GPIO_IN_OFFSET              (0x0008u) /* Input data register (RO) */
#define GPIO_SET_OFFSET             (0x000Cu) /* Atomic set bits (WO) */
#define GPIO_CLR_OFFSET             (0x0010u) /* Atomic clear bits (WO) */
#define GPIO_IRQ_EN_OFFSET          (0x0014u) /* Interrupt enable */
#define GPIO_IRQ_TYPE_OFFSET        (0x0018u) /* 0=level, 1=edge */
#define GPIO_IRQ_POL_OFFSET         (0x001Cu) /* 0=low/falling, 1=high/rising */
#define GPIO_IRQ_STATUS_OFFSET      (0x0020u) /* W1C interrupt status */
#define GPIO_DEBOUNCE_OFFSET        (0x0024u) /* Debounce enable */
#define GPIO_PULL_CFG_OFFSET        (0x0028u) /* Pull-up/down configuration */

/* Absolute register addresses */
#define GPIO_REG_ADDR(base, off)    ((uintptr_t)((base) + (uintptr_t)(off)))
#define GPIO_DIR_ADDR               GPIO_REG_ADDR(TMS320C6452_DSP_GPIO_BASE, GPIO_DIR_OFFSET)
#define GPIO_OUT_ADDR               GPIO_REG_ADDR(TMS320C6452_DSP_GPIO_BASE, GPIO_OUT_OFFSET)
#define GPIO_IN_ADDR                GPIO_REG_ADDR(TMS320C6452_DSP_GPIO_BASE, GPIO_IN_OFFSET)
#define GPIO_SET_ADDR               GPIO_REG_ADDR(TMS320C6452_DSP_GPIO_BASE, GPIO_SET_OFFSET)
#define GPIO_CLR_ADDR               GPIO_REG_ADDR(TMS320C6452_DSP_GPIO_BASE, GPIO_CLR_OFFSET)
#define GPIO_IRQ_EN_ADDR            GPIO_REG_ADDR(TMS320C6452_DSP_GPIO_BASE, GPIO_IRQ_EN_OFFSET)
#define GPIO_IRQ_TYPE_ADDR          GPIO_REG_ADDR(TMS320C6452_DSP_GPIO_BASE, GPIO_IRQ_TYPE_OFFSET)
#define GPIO_IRQ_POL_ADDR           GPIO_REG_ADDR(TMS320C6452_DSP_GPIO_BASE, GPIO_IRQ_POL_OFFSET)
#define GPIO_IRQ_STATUS_ADDR        GPIO_REG_ADDR(TMS320C6452_DSP_GPIO_BASE, GPIO_IRQ_STATUS_OFFSET)
#define GPIO_DEBOUNCE_ADDR          GPIO_REG_ADDR(TMS320C6452_DSP_GPIO_BASE, GPIO_DEBOUNCE_OFFSET)
#define GPIO_PULL_CFG_ADDR          GPIO_REG_ADDR(TMS320C6452_DSP_GPIO_BASE, GPIO_PULL_CFG_OFFSET)

/* Bit helpers */
#define GPIO_PIN(bit)               ((uint32_t)1u << ((uint32_t)(bit) & 31u))

/* Forward declarations for core helpers */
uint32_t gpio_reg_read32(uintptr_t addr);
void     gpio_reg_write32(uintptr_t addr, uint32_t val);
void     gpio_reg_set_bits(uintptr_t addr, uint32_t mask);
void     gpio_reg_clear_bits(uintptr_t addr, uint32_t mask);
void     gpio_reg_update_bits(uintptr_t addr, uint32_t mask, uint32_t value);

/* Core ops interface for feature drivers */
typedef uint32_t (*gpio_read32_t)(uintptr_t addr);
typedef void     (*gpio_write32_t)(uintptr_t addr, uint32_t val);
typedef void     (*gpio_bits_t)(uintptr_t addr, uint32_t mask);
typedef void     (*gpio_update_bits_t)(uintptr_t addr, uint32_t mask, uint32_t value);

struct gpio_core_ops {
    gpio_read32_t        read32;       /* 32-bit read (device-endian aware) */
    gpio_write32_t       write32;      /* 32-bit write (device-endian aware) */
    gpio_bits_t          set_bits;     /* Atomic-as-possible bit set (RMW) */
    gpio_bits_t          clear_bits;   /* Atomic-as-possible bit clear (RMW) */
    gpio_update_bits_t   update_bits;  /* Masked update (RMW) */
    uintptr_t            base;         /* Base address of GPIO block */
    uint8_t              device_is_be; /* 1 if device registers are big-endian */
};

const struct gpio_core_ops* gpio_core_get_ops(void);

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_DSP_CORE_H */
