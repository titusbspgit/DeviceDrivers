#ifndef TMS320C6452_GPIO_GPIO_BANKED_ARCHITECTURE_32_SIGNALS_2_BANKS__DRV_H
#define TMS320C6452_GPIO_GPIO_BANKED_ARCHITECTURE_32_SIGNALS_2_BANKS__DRV_H

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

/* Base address (UNKNOWN placeholder) */
#define TMS320C6452_GPIO_BASE_ADDR UNKNOWN

/* Architecture constants */
#define TMS320C6452_GPIO_TOTAL_PINS        (32u)
#define TMS320C6452_GPIO_BANK_COUNT        (2u)
#define TMS320C6452_GPIO_PINS_PER_BANK     (16u)

/* Bank/bit helpers as macros */
#define TMS320C6452_GPIO_PIN_TO_BANK(pin)  (((uint32_t)(pin) >> 4) & 0x1u)
#define TMS320C6452_GPIO_PIN_TO_BIT(pin)   ((uint32_t)(1u << ((uint32_t)(pin) & 0x0Fu)))

/* Error codes */
#define TMS320C6452_GPIO_EOK           (0)
#define TMS320C6452_GPIO_EINVAL        (-1)
#define TMS320C6452_GPIO_EUNSUPPORTED  (-2)

/* Driver context for banked architecture */
typedef struct {
    uintptr_t base; /* GPIO module base address (UNKNOWN if not provided) */
} tms320c6452_gpio_banked_ctx_t;

/* APIs (single feature: banked architecture only) */
int tms320c6452_gpio_banked_init(tms320c6452_gpio_banked_ctx_t* ctx, uintptr_t gpio_base);
int tms320c6452_gpio_banked_is_valid_pin(uint32_t pin);
int tms320c6452_gpio_banked_pin_to_bank(uint32_t pin, uint32_t* out_bank, uint32_t* out_bit_pos);
uint32_t tms320c6452_gpio_banked_pin_mask(uint32_t pin);

/* Generic banked register access helpers (offsets UNKNOWN) */
int tms320c6452_gpio_banked_reg_read(const tms320c6452_gpio_banked_ctx_t* ctx, uintptr_t reg_offset, uint32_t* out_val);
int tms320c6452_gpio_banked_reg_write(const tms320c6452_gpio_banked_ctx_t* ctx, uintptr_t reg_offset, uint32_t val);
int tms320c6452_gpio_banked_reg_update(const tms320c6452_gpio_banked_ctx_t* ctx, uintptr_t reg_offset, uint32_t mask, uint32_t val);

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_GPIO_GPIO_BANKED_ARCHITECTURE_32_SIGNALS_2_BANKS__DRV_H */
