/*
 * Feature: GPIO banked architecture (32 signals, 2 banks)
 * Controller: TMS320C6452 DSP
 *
 * Single-feature header. Depends on TMS320C6452 DSP_core.h
 */
#ifndef TMS320C6452_DSP_GPIO_BANKED_ARCHITECTURE_32_SIGNALS_2_BANKS_H
#define TMS320C6452_DSP_GPIO_BANKED_ARCHITECTURE_32_SIGNALS_2_BANKS_H

#include <stdint.h>
#include "TMS320C6452 DSP_core.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GPIO_BANK_COUNT   (2u)
#define GPIO_PIN_COUNT    (32u)

/* Register organization is device-specific and not provided; mark unknown */
#define GPIO_BANK_REG_MAP_UNKNOWN 1

/* APIs specific to this feature */
typedef struct {
    uint32_t bank; /* 0..1 */
    uint32_t bit;  /* 0..15 */
} gpio_bank_bit_t;

/* Map a GPIO pin number [0..31] to bank and bit. */
gpio_drv_status_t gpio_arch_map_pin(uint32_t pin, gpio_bank_bit_t *out);

/* Optional bulk iterators (do not touch hardware) */
gpio_drv_status_t gpio_arch_first_in_bank(uint32_t bank, uint32_t *first_pin);

gpio_drv_status_t gpio_arch_next_in_bank(uint32_t bank, uint32_t prev_pin, uint32_t *next_pin);

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_DSP_GPIO_BANKED_ARCHITECTURE_32_SIGNALS_2_BANKS_H */
