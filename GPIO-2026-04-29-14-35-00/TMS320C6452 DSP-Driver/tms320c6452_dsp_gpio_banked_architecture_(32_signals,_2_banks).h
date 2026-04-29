/*
 * Feature: GPIO banked architecture (32 signals, 2 banks)
 * Controller: TMS320C6452 DSP
 * Environment: Baremetal, gcc, big-endian
 * Constraint: No dynamic allocation; deterministic MMIO via core
 */
#ifndef TMS320C6452_DSP_GPIO_BANKED_ARCHITECTURE_32_SIGNALS__2_BANKS__H
#define TMS320C6452_DSP_GPIO_BANKED_ARCHITECTURE_32_SIGNALS__2_BANKS__H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Core dependency: register macros and MMIO helpers */
#include "TMS320C6452 DSP_core.h"

/* Public feature constants */
#define TMS320C6452_DSP_GPIO_FEATURE_MAX_PINS        (32U)
#define TMS320C6452_DSP_GPIO_FEATURE_PINS_PER_BANK  (16U)
#define TMS320C6452_DSP_GPIO_FEATURE_NUM_BANKS      (2U)

/* Status codes for feature APIs */
typedef enum
{
    TMS320C6452_DSP_GPIO_FEAT_OK = 0,
    TMS320C6452_DSP_GPIO_FEAT_ERR_PARAM = 1,
    TMS320C6452_DSP_GPIO_FEAT_ERR_UNSUPPORTED = 2
} tms320c6452_dsp_gpio_feat_status_t;

/* Compute bank index [0..1] from a GPIO pin number [0..31] */
static inline uint32_t tms320c6452_dsp_gpio_bank_index_from_pin(uint32_t pin)
{
    return (pin >> 4) & 0x1U; /* 16 pins per bank */
}

/* Compute bit mask within a bank from a GPIO pin number [0..31] */
static inline uint32_t tms320c6452_dsp_gpio_bitmask_from_pin(uint32_t pin)
{
    return (uint32_t)(1UL << (pin & 0xFU));
}

/* Compute bank and bit mask from a pin. Returns OK on success. */
static inline tms320c6452_dsp_gpio_feat_status_t tms320c6452_dsp_gpio_pin_to_bank_bit(uint32_t pin, uint32_t *bank, uint32_t *bitmask)
{
    if ((pin >= TMS320C6452_DSP_GPIO_FEATURE_MAX_PINS) || (bank == NULL) || (bitmask == NULL))
    {
        return TMS320C6452_DSP_GPIO_FEAT_ERR_PARAM;
    }
    *bank = tms320c6452_dsp_gpio_bank_index_from_pin(pin);
    *bitmask = tms320c6452_dsp_gpio_bitmask_from_pin(pin);
    return TMS320C6452_DSP_GPIO_FEAT_OK;
}

/* Iterate over all banks [0..1] and invoke callback. */
static inline void tms320c6452_dsp_gpio_for_each_bank(void (*cb)(uint32_t bank, void *user), void *user)
{
    if (cb != NULL)
    {
        cb(0U, user);
        cb(1U, user);
    }
}

/* Initialization stub for the architecture feature; ensures core is present. */
int tms320c6452_dsp_gpio_arch_init(void);

/* Bank-level helpers using core operations. These may return UNSUPPORTED if core does not expose bank operations. */
int tms320c6452_dsp_gpio_bank_config_dir(uint32_t bank, uint32_t dir_mask);
int tms320c6452_dsp_gpio_bank_read_in(uint32_t bank, uint32_t *value);
int tms320c6452_dsp_gpio_bank_read_out(uint32_t bank, uint32_t *value);
int tms320c6452_dsp_gpio_bank_set_mask(uint32_t bank, uint32_t mask);
int tms320c6452_dsp_gpio_bank_clear_mask(uint32_t bank, uint32_t mask);

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_DSP_GPIO_BANKED_ARCHITECTURE_32_SIGNALS__2_BANKS__H */
