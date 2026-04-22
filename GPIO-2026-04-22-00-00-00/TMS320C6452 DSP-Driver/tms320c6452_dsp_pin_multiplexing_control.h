/*
 * Feature: Pin multiplexing control
 * Controller: TMS320C6452 DSP
 *
 * Single-feature header. Depends on TMS320C6452 DSP_core.h
 */
#ifndef TMS320C6452_DSP_PIN_MULTIPLEXING_CONTROL_H
#define TMS320C6452_DSP_PIN_MULTIPLEXING_CONTROL_H

#include <stdint.h>
#include "TMS320C6452 DSP_core.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Pinmux is device-wide and outside GPIO; register details are UNKNOWN here. */
#define PINMUX_CONFIG_REG_UNKNOWN 1

/* APIs specific to this feature */

gpio_drv_status_t gpio_pinmux_validate(uint32_t gpio_pin);

gpio_drv_status_t gpio_pinmux_configure(uint32_t gpio_pin, uint32_t mux_sel);

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_DSP_PIN_MULTIPLEXING_CONTROL_H */
