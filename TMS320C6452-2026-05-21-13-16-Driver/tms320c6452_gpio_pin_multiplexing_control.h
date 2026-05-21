#ifndef TMS320C6452_GPIO_PIN_MULTIPLEXING_CONTROL_H
#define TMS320C6452_GPIO_PIN_MULTIPLEXING_CONTROL_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Target Environment (UNKNOWN placeholders) */
#define TMS320C6452_GPIO_CPU_ARCH UNKNOWN
#define TMS320C6452_GPIO_ENVIRONMENT UNKNOWN
#define TMS320C6452_GPIO_COMPILER UNKNOWN
#define TMS320C6452_GPIO_ENDIANNESS UNKNOWN

/* System/Device Configuration base (SYSCFG/DEV) for PINMUX access */
#define TMS320C6452_GPIO_SYSCFG_BASE UNKNOWN

/* PINMUX register model (device-specific; UNKNOWN without data manual) */
#define TMS320C6452_GPIO_PINMUX_REG_COUNT UNKNOWN
#define TMS320C6452_GPIO_PINMUX_FIELD_BITS UNKNOWN /* typically 2 bits per field; exact per data manual */

/* KICK lock/unlock mechanism (device-specific; UNKNOWN without manual) */
#define TMS320C6452_GPIO_KICK0_ADDR UNKNOWN
#define TMS320C6452_GPIO_KICK1_ADDR UNKNOWN
#define TMS320C6452_GPIO_KICK0_UNLOCK_KEY UNKNOWN
#define TMS320C6452_GPIO_KICK1_UNLOCK_KEY UNKNOWN
#define TMS320C6452_GPIO_KICK_LOCK_KEY  UNKNOWN

/* Register addressing helpers (offsets UNKNOWN without data manual) */
#define TMS320C6452_GPIO_PINMUX_REG_OFFSET(n) UNKNOWN
#define TMS320C6452_GPIO_PINMUX_REG_ADDR(base, n) ((volatile uint32_t*)((uintptr_t)(base) + (uintptr_t)(TMS320C6452_GPIO_PINMUX_REG_OFFSET(n))))

/* Error codes */
#define TMS320C6452_GPIO_EOK           (0)
#define TMS320C6452_GPIO_EINVAL        (-1)
#define TMS320C6452_GPIO_EUNSUPPORTED  (-2)
#define TMS320C6452_GPIO_EFAULT        (-3)

/* Driver context */
typedef struct {
    uintptr_t syscfg_base; /* SYSCFG/DEV base; required for PINMUX access */
} tms320c6452_gpio_pinmux_ctx_t;

/* API declarations (single feature: Pin multiplexing control) */
int tms320c6452_gpio_pin_multiplexing_control_init(tms320c6452_gpio_pinmux_ctx_t* ctx, uintptr_t syscfg_base);
int tms320c6452_gpio_pin_multiplexing_control_unlock(const tms320c6452_gpio_pinmux_ctx_t* ctx);
int tms320c6452_gpio_pin_multiplexing_control_lock(const tms320c6452_gpio_pinmux_ctx_t* ctx);
int tms320c6452_gpio_pin_multiplexing_control_set(const tms320c6452_gpio_pinmux_ctx_t* ctx, uint32_t pin_index, uint32_t function_sel);
int tms320c6452_gpio_pin_multiplexing_control_get(const tms320c6452_gpio_pinmux_ctx_t* ctx, uint32_t pin_index, uint32_t* out_function_sel);

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_GPIO_PIN_MULTIPLEXING_CONTROL_H */
