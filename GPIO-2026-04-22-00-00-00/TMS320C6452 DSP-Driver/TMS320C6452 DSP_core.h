/*
 * TMS320C6452 DSP GPIO Core Driver (u-boot)
 *
 * All GPIO feature drivers for this controller must include this header
 * and use the provided gpio_core_ops instance for MMIO access.
 *
 * Controller: TMS320C6452 DSP
 * Environment: uboot
 * Endianness: bigendian
 */
#ifndef TMS320C6452_DSP_CORE_H
#define TMS320C6452_DSP_CORE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

/* Base address for GPIO module (from configuration) */
#define TMS320C6452_DSP_GPIO_BASE   ( (uintptr_t)0xA1008000u )

/* Register placeholder macros (offsets unknown without device manual) */
#define GPIO_UNKNOWN_REG0_OFFSET    /* UNKNOWN */
#define GPIO_UNKNOWN_REG1_OFFSET    /* UNKNOWN */

/* Common masks and helpers */
#define GPIO_PIN_MASK(pin)          ( (uint32_t)(1u << ((uint32_t)(pin) & 31u)) )
#define GPIO_IS_VALID_PIN(pin)      ( ((uint32_t)(pin)) < 32u )

/* Error codes for feature drivers using this core */
typedef enum {
    GPIO_DRV_OK = 0,
    GPIO_DRV_ERR_INVALID_PARAM = -1,
    GPIO_DRV_ERR_UNSUPPORTED   = -2,
    GPIO_DRV_ERR_UNKNOWN_REG   = -3
} gpio_drv_status_t;

/* GPIO core ops interface for feature drivers */
typedef struct gpio_core_ops_s {
    /* base is captured internally by core_init */
    void     (*core_init)(uintptr_t base);
    uint32_t (*read_reg)(uintptr_t offset);
    void     (*write_reg)(uintptr_t offset, uint32_t value);
    void     (*set_bits)(uintptr_t offset, uint32_t mask);
    void     (*clear_bits)(uintptr_t offset, uint32_t mask);
    void     (*read_modify_write)(uintptr_t offset, uint32_t mask, uint32_t value);
    void     (*delay_us)(uint32_t usec);
} gpio_core_ops_t;

/* Public instance implemented in core driver C file */
extern const gpio_core_ops_t gpio_core_ops;

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_DSP_CORE_H */
