/*
 * TMS320C6452 DSP GPIO Core - reusable core for feature drivers
 * Environment: U-Boot (assumed), C99, big-endian register space
 * NOTE: Register offsets below are scaffolds. TODO: replace with TRM-verified offsets.
 */
#ifndef TMS320C6452_DSP_CORE_H
#define TMS320C6452_DSP_CORE_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Base address (from configuration) */
#define TMS320C6452_DSP_GPIO_BASE   ((uintptr_t)0xA1008000u)

/* GPIO register offsets (PLACEHOLDERS — TODO verify with device TRM) */
#define GPIO_DIR01_OFFSET       (0x0000u) /* TODO */
#define GPIO_OUT_DATA01_OFFSET  (0x0004u) /* TODO */
#define GPIO_IN_DATA01_OFFSET   (0x0008u) /* TODO */
#define GPIO_SET_DATA01_OFFSET  (0x000Cu) /* TODO */
#define GPIO_CLR_DATA01_OFFSET  (0x0010u) /* TODO */
#define GPIO_BINTEN_OFFSET      (0x0014u) /* TODO: Bank interrupt enable (bit0: bank0, bit1: bank1) */

#define GPIO_MAX_PINS           (32u)
#define GPIO_PINS_PER_BANK      (16u)
#define GPIO_NUM_BANKS          (2u)

/* Forward declarations */
struct gpio_core;
struct gpio_core_api;

/*
 * Handle storing base and endianness. Feature drivers must not access fields directly.
 */
typedef struct gpio_core {
    volatile uint8_t *base;   /* MMIO base */
    bool big_endian_regs;     /* true if registers are big-endian */
} gpio_core_t;

/* Function table exposed to feature drivers */
typedef struct gpio_core_api {
    /* Raw register access (offset-based) */
    uint32_t (*reg_read32)(gpio_core_t *ctx, uintptr_t off);
    void     (*reg_write32)(gpio_core_t *ctx, uintptr_t off, uint32_t val);

    /* Direction: 1=input, 0=output */
    int  (*set_dir)(gpio_core_t *ctx, unsigned pin, bool is_input);
    int  (*get_dir)(gpio_core_t *ctx, unsigned pin, bool *is_input);

    /* Pin I/O */
    int  (*write_pin)(gpio_core_t *ctx, unsigned pin, bool value);
    int  (*read_pin)(gpio_core_t *ctx, unsigned pin, bool *value);

    /* Bitfield ops across 32 pins */
    int  (*set_bits)(gpio_core_t *ctx, uint32_t mask);
    int  (*clear_bits)(gpio_core_t *ctx, uint32_t mask);
    int  (*toggle_bits)(gpio_core_t *ctx, uint32_t mask);

    /* Bank interrupt enable */
    int  (*enable_bank_irq)(gpio_core_t *ctx, unsigned bank, bool enable);
} gpio_core_api_t;

/*
 * Initialize core driver. base may be NULL to use compiled-in base.
 * big_endian_regs should be true for this device (assumed big-endian register map).
 * Returns pointer to static function table; ctx must persist for API lifetime.
 */
const gpio_core_api_t *gpio_core_init(gpio_core_t *ctx, void *base, bool big_endian_regs);

/* Helpers (header-inline) */
static inline unsigned gpio_bank_from_pin(unsigned pin)
{
    return (pin >> 4) & (GPIO_NUM_BANKS - 1u);
}

static inline uint32_t gpio_bit_from_pin(unsigned pin)
{
    return (pin < GPIO_MAX_PINS) ? (uint32_t)(1u << (pin & 31u)) : 0u;
}

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_DSP_CORE_H */
