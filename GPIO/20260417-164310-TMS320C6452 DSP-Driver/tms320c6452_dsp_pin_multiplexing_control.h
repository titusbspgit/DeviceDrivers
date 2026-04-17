#ifndef TMS320C6452_DSP_PIN_MULTIPLEXING_CONTROL_H
#define TMS320C6452_DSP_PIN_MULTIPLEXING_CONTROL_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Pin multiplexing control lies outside GPIO; details UNKNOWN */
#define TMS320C6452_DSP_PINMUX_BASE           /* UNKNOWN */
#define TMS320C6452_DSP_PINMUX_REG_OFFSET     /* UNKNOWN */

bool tms320c6452_dsp_pin_multiplexing_control_validate(uint32_t pin);
bool tms320c6452_dsp_pin_multiplexing_control_configure_gpio(uint32_t pin);
int  tms320c6452_dsp_pin_multiplexing_control_query_function(uint32_t pin);

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_DSP_PIN_MULTIPLEXING_CONTROL_H */
