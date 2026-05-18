#ifndef TMS320C6452_DSP_GPIO_BANKED_ARCHITECTURE__32_SIGNALS__2_BANKS_H
#define TMS320C6452_DSP_GPIO_BANKED_ARCHITECTURE__32_SIGNALS__2_BANKS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Controller base address (MMIO) */
#define TMS320C6452_DSP_GPIO_BASE_ADDR   ((uintptr_t)0xA1008000u)

/* Banked architecture parameters */
#define TMS320C6452_DSP_GPIO_BANK_COUNT    (2u)
#define TMS320C6452_DSP_GPIO_PINS_PER_BANK (16u)
#define TMS320C6452_DSP_GPIO_TOTAL_PINS    (TMS320C6452_DSP_GPIO_BANK_COUNT * TMS320C6452_DSP_GPIO_PINS_PER_BANK)

/* Pin mapping helpers */
#define TMS320C6452_DSP_GPIO_PIN_TO_BANK(pin)   ((uint32_t)((pin) / TMS320C6452_DSP_GPIO_PINS_PER_BANK))
#define TMS320C6452_DSP_GPIO_PIN_TO_BIT(pin)    ((uint32_t)((pin) % TMS320C6452_DSP_GPIO_PINS_PER_BANK))
#define TMS320C6452_DSP_GPIO_BIT(n)             (UINT32_C(1) << (uint32_t)(n))

/* Unknown register/stride sentinel */
#define TMS320C6452_DSP_GPIO_REG_UNKNOWN  ((uintptr_t)(~(uintptr_t)0))

/* Register offsets (relative to base) — use UNKNOWN when not available */
#define TMS320C6452_DSP_GPIO_BANK_STRIDE_OFFSET   TMS320C6452_DSP_GPIO_REG_UNKNOWN /* UNKNOWN */
#define TMS320C6452_DSP_GPIO_DIR_OFFSET           TMS320C6452_DSP_GPIO_REG_UNKNOWN /* UNKNOWN: Direction per bank (1=output,0=input) */
#define TMS320C6452_DSP_GPIO_OUT_DATA_OFFSET      TMS320C6452_DSP_GPIO_REG_UNKNOWN /* UNKNOWN: Output data per bank */
#define TMS320C6452_DSP_GPIO_IN_DATA_OFFSET       TMS320C6452_DSP_GPIO_REG_UNKNOWN /* UNKNOWN: Input data per bank */
#define TMS320C6452_DSP_GPIO_SET_DATA_OFFSET      TMS320C6452_DSP_GPIO_REG_UNKNOWN /* UNKNOWN: Atomic set per bank */
#define TMS320C6452_DSP_GPIO_CLR_DATA_OFFSET      TMS320C6452_DSP_GPIO_REG_UNKNOWN /* UNKNOWN: Atomic clear per bank */
#define TMS320C6452_DSP_GPIO_INT_EN_OFFSET        TMS320C6452_DSP_GPIO_REG_UNKNOWN /* UNKNOWN: Interrupt enable per bank */
#define TMS320C6452_DSP_GPIO_INT_STS_OFFSET       TMS320C6452_DSP_GPIO_REG_UNKNOWN /* UNKNOWN: Interrupt status per bank */
#define TMS320C6452_DSP_GPIO_INT_CLR_OFFSET       TMS320C6452_DSP_GPIO_REG_UNKNOWN /* UNKNOWN: Interrupt clear per bank */

/* Status codes */
typedef enum
{
    TMS320C6452_DSP_GPIO_OK = 0,
    TMS320C6452_DSP_GPIO_ERR_PARAM = -1,
    TMS320C6452_DSP_GPIO_ERR_UNSUPPORTED = -2
} tms320c6452_dsp_gpio_status_t;

/* Feature configuration for banked architecture */
typedef struct
{
    uintptr_t base;            /* Base MMIO address */
    uintptr_t bank_stride;     /* Per-bank register block stride; UNKNOWN if not available */

    /* Per-bank register offsets relative to base; UNKNOWN if not available */
    uintptr_t dir_offset;
    uintptr_t in_offset;
    uintptr_t out_offset;
    uintptr_t set_offset;
    uintptr_t clr_offset;
    uintptr_t int_en_offset;
    uintptr_t int_sts_offset;
    uintptr_t int_clr_offset;
} tms320c6452_dsp_gpio_banked_cfg_t;

/* Runtime context */
typedef struct
{
    tms320c6452_dsp_gpio_banked_cfg_t cfg;
} tms320c6452_dsp_gpio_banked_ctx_t;

/* Initialization */
tms320c6452_dsp_gpio_status_t
tms320c6452_dsp_gpio_banked_init(tms320c6452_dsp_gpio_banked_ctx_t* ctx,
                                 const tms320c6452_dsp_gpio_banked_cfg_t* cfg);

/* Bank-level APIs */
tms320c6452_dsp_gpio_status_t
tms320c6452_dsp_gpio_banked_config_dir_bank(tms320c6452_dsp_gpio_banked_ctx_t* ctx,
                                            uint32_t bank,
                                            uint32_t mask,
                                            bool is_output);

tms320c6452_dsp_gpio_status_t
tms320c6452_dsp_gpio_banked_write_bank_mask(tms320c6452_dsp_gpio_banked_ctx_t* ctx,
                                            uint32_t bank,
                                            uint32_t set_mask,
                                            uint32_t clear_mask);

tms320c6452_dsp_gpio_status_t
tms320c6452_dsp_gpio_banked_read_bank(tms320c6452_dsp_gpio_banked_ctx_t* ctx,
                                      uint32_t bank,
                                      uint32_t* mask);

/* Pin-level convenience APIs using banked mapping */
tms320c6452_dsp_gpio_status_t
tms320c6452_dsp_gpio_banked_write_pin(tms320c6452_dsp_gpio_banked_ctx_t* ctx,
                                      uint32_t pin,
                                      bool value);

tms320c6452_dsp_gpio_status_t
tms320c6452_dsp_gpio_banked_read_pin(tms320c6452_dsp_gpio_banked_ctx_t* ctx,
                                     uint32_t pin,
                                     bool* value);

/* Optional interrupt helpers (per bank). Unsupported if offsets UNKNOWN. */
tms320c6452_dsp_gpio_status_t
tms320c6452_dsp_gpio_banked_irq_enable_bank(tms320c6452_dsp_gpio_banked_ctx_t* ctx,
                                            uint32_t bank,
                                            uint32_t enable_mask);

tms320c6452_dsp_gpio_status_t
tms320c6452_dsp_gpio_banked_irq_clear_bank(tms320c6452_dsp_gpio_banked_ctx_t* ctx,
                                           uint32_t bank,
                                           uint32_t clear_mask);

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_DSP_GPIO_BANKED_ARCHITECTURE__32_SIGNALS__2_BANKS_H */
