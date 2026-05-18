#ifndef TMS320C6452_DSP_CORE_H
#define TMS320C6452_DSP_CORE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define TMS320C6452_DSP_CORE_REG_UNKNOWN ((uintptr_t)(~(uintptr_t)0))

#define TMS320C6452_DSP_GPIO_BASE_ADDR   ((uintptr_t)0xA1008000u)
#define TMS320C6452_DSP_GPIO_BANK_COUNT    (2u)
#define TMS320C6452_DSP_GPIO_PINS_PER_BANK (16u)
#define TMS320C6452_DSP_GPIO_TOTAL_PINS    (TMS320C6452_DSP_GPIO_BANK_COUNT * TMS320C6452_DSP_GPIO_PINS_PER_BANK)

#define TMS320C6452_DSP_GPIO_PIN_TO_BANK(pin)   ((uint32_t)((pin) / TMS320C6452_DSP_GPIO_PINS_PER_BANK))
#define TMS320C6452_DSP_GPIO_PIN_TO_BIT(pin)    ((uint32_t)((pin) % TMS320C6452_DSP_GPIO_PINS_PER_BANK))
#define TMS320C6452_DSP_GPIO_BIT(n)             (UINT32_C(1) << (uint32_t)(n))

/* Offsets/stride for banked registers (UNKNOWN by default) */
typedef struct
{
    uintptr_t base;
    uintptr_t bank_stride;
    uintptr_t dir_offset;
    uintptr_t in_offset;
    uintptr_t out_offset;
    uintptr_t set_offset;
    uintptr_t clr_offset;
    uintptr_t int_en_offset;
    uintptr_t int_sts_offset;
    uintptr_t int_clr_offset;
} tms320c6452_dsp_gpio_core_ctx_t;

/* Return codes for core ops */
typedef enum {
    TMS320C6452_DSP_CORE_OK = 0,
    TMS320C6452_DSP_CORE_ERR_PARAM = -1,
    TMS320C6452_DSP_CORE_ERR_UNSUPPORTED = -2
} tms320c6452_dsp_core_status_t;

/* Ops table exposed to feature drivers */
typedef struct tms320c6452_dsp_gpio_core_ops_s {
    uint32_t (*read32)(const tms320c6452_dsp_gpio_core_ctx_t* ctx, uintptr_t offset, uint32_t bank);
    void     (*write32)(const tms320c6452_dsp_gpio_core_ctx_t* ctx, uintptr_t offset, uint32_t bank, uint32_t value);

    tms320c6452_dsp_core_status_t (*set_bits)(tms320c6452_dsp_gpio_core_ctx_t* ctx, uint32_t bank, uint32_t mask);
    tms320c6452_dsp_core_status_t (*clear_bits)(tms320c6452_dsp_gpio_core_ctx_t* ctx, uint32_t bank, uint32_t mask);
    tms320c6452_dsp_core_status_t (*read_bits)(tms320c6452_dsp_gpio_core_ctx_t* ctx, uint32_t bank, uint32_t* value);
    tms320c6452_dsp_core_status_t (*config_dir)(tms320c6452_dsp_gpio_core_ctx_t* ctx, uint32_t bank, uint32_t mask, bool is_output);

    tms320c6452_dsp_core_status_t (*write_pin)(tms320c6452_dsp_gpio_core_ctx_t* ctx, uint32_t pin, bool value);
    tms320c6452_dsp_core_status_t (*read_pin)(tms320c6452_dsp_gpio_core_ctx_t* ctx, uint32_t pin, bool* value);
} tms320c6452_dsp_gpio_core_ops_t;

/* Initialize context and bind ops; offsets/stride left as UNKNOWN unless caller assigns */
tms320c6452_dsp_core_status_t tms320c6452_dsp_gpio_core_init(
    tms320c6452_dsp_gpio_core_ctx_t* ctx,
    tms320c6452_dsp_gpio_core_ops_t* ops,
    uintptr_t base);

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_DSP_CORE_H */
