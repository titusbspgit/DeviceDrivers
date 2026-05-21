#ifndef TMS320C6452_PIN_MULTIPLEXING_CONTROL_H
#define TMS320C6452_PIN_MULTIPLEXING_CONTROL_H

#include <stdint.h>
#include <stdbool.h>
#include "tms320c6452_gpio_core.h"

/* Feature: Pin multiplexing control */
/* NOTE: Pinmux registers are outside GPIO and are UNKNOWN in this context. */

/* Validate that pinmux is configured for GPIO function. */
int tms320c6452_pinmux_validate(void);

/* Attempt to configure default GPIO pinmux if supported. */
int tms320c6452_pinmux_configure_default(void);

/* Get mux setting for a specific pin (0..31). */
int tms320c6452_pinmux_get(uint32_t pin, uint32_t *mux_val);

#endif /* TMS320C6452_PIN_MULTIPLEXING_CONTROL_H */
