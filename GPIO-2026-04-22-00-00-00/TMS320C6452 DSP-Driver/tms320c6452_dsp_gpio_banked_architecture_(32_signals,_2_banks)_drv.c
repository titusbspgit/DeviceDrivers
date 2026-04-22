/*
 * Feature Driver: GPIO banked architecture (32 signals, 2 banks)
 *
 * Provides pin->bank/bit mapping utilities. No register access performed here.
 */
#include <stddef.h>
#include "tms320c6452_dsp_gpio_banked_architecture_(32_signals,_2_banks).h"

/* No hardware register access required; only mapping helpers. */

gpio_drv_status_t gpio_arch_map_pin(uint32_t pin, gpio_bank_bit_t *out)
{
    if (!GPIO_IS_VALID_PIN(pin) || (out == NULL)) {
        return GPIO_DRV_ERR_INVALID_PARAM;
    }
    out->bank = (pin >> 4) & 0x1u; /* 0..1, 16 pins per bank */
    out->bit  = pin & 0xFu;        /* 0..15 */
    return GPIO_DRV_OK;
}

gpio_drv_status_t gpio_arch_first_in_bank(uint32_t bank, uint32_t *first_pin)
{
    if ((bank >= GPIO_BANK_COUNT) || (first_pin == NULL)) {
        return GPIO_DRV_ERR_INVALID_PARAM;
    }
    *first_pin = bank * 16u;
    return GPIO_DRV_OK;
}

gpio_drv_status_t gpio_arch_next_in_bank(uint32_t bank, uint32_t prev_pin, uint32_t *next_pin)
{
    if ((bank >= GPIO_BANK_COUNT) || (next_pin == NULL)) {
        return GPIO_DRV_ERR_INVALID_PARAM;
    }
    uint32_t base = bank * 16u;
    if (prev_pin < base || prev_pin >= (base + 16u - 1u)) {
        return GPIO_DRV_ERR_INVALID_PARAM;
    }
    *next_pin = prev_pin + 1u;
    return GPIO_DRV_OK;
}
