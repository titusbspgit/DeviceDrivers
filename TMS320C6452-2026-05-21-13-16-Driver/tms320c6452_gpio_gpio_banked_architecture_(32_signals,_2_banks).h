#ifndef TMS320C6452_GPIO_GPIO_BANKED_ARCHITECTURE_32_SIGNALS_2_BANKS_H
#define TMS320C6452_GPIO_GPIO_BANKED_ARCHITECTURE_32_SIGNALS_2_BANKS_H

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

/* Controller Name: TMS320C6452 GPIO */
/* Feature: GPIO banked architecture (32 signals, 2 banks) */

/* GPIO base address (memory-mapped I/O) */
#define TMS320C6452_GPIO_BASE UNKNOWN

/* Architectural constants */
#define TMS320C6452_GPIO_BANK_COUNT        (2u)
#define TMS320C6452_GPIO_PIN_COUNT         (32u)
#define TMS320C6452_GPIO_PINS_PER_BANK     (16u)

/* Error codes */
#define TMS320C6452_GPIO_EOK           (0)
#define TMS320C6452_GPIO_EINVAL        (-1)
#define TMS320C6452_GPIO_EFAULT        (-2)
#define TMS320C6452_GPIO_EUNSUPPORTED  (-3)

/* Driver context: base address for banked 32-bit registers */
typedef struct {
    uintptr_t base; /* Base of GPIO register block controlling 32 pins across 2 banks */
} tms320c6452_gpio_gpio_banked_architecture_ctx_t;

/* API declarations (single feature: GPIO banked architecture) */
int tms320c6452_gpio_gpio_banked_architecture_init(tms320c6452_gpio_gpio_banked_architecture_ctx_t* ctx, uintptr_t gpio_base);
int tms320c6452_gpio_gpio_banked_architecture_pin_to_bank(uint32_t pin_index, uint32_t* out_bank_index, uint32_t* out_bit_index);
int tms320c6452_gpio_gpio_banked_architecture_pin_to_mask(uint32_t pin_index, uint32_t* out_mask);
int tms320c6452_gpio_gpio_banked_architecture_read32(const tms320c6452_gpio_gpio_banked_architecture_ctx_t* ctx, uint32_t reg_offset_bytes, uint32_t* out_val);
int tms320c6452_gpio_gpio_banked_architecture_write32(const tms320c6452_gpio_gpio_banked_architecture_ctx_t* ctx, uint32_t reg_offset_bytes, uint32_t val);
int tms320c6452_gpio_gpio_banked_architecture_update_bits(const tms320c6452_gpio_gpio_banked_architecture_ctx_t* ctx, uint32_t reg_offset_bytes, uint32_t mask, uint32_t value_masked);

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_GPIO_GPIO_BANKED_ARCHITECTURE_32_SIGNALS_2_BANKS_H */
