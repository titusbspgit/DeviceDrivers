#ifndef TMS320C6452_DSP_PIN_MUX_CONTROL_H
#define TMS320C6452_DSP_PIN_MUX_CONTROL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/* Core ops (types/barriers). This driver does not touch GPIO regs directly. */
#include "TMS320C6452 DSP_core.h"

/* Error codes */
#define GPIO_EOK        (0)
#define GPIO_EINVAL     (-22)

/* Constraints */
#define GPIO_PINMUX_MAX_PINS   (32u)

/* Pinmux MMIO definitions: UNKNOWN for this device in this context.
   When real offsets are available, replace these UNKNOWN markers and
   the implementation will switch from table mode to MMIO mode. */
#define PINMUX_BASE_OFFSET_UNKNOWN      (0xFFFFFFFFu)
#define PINMUX_FUNC_SEL_STRIDE_UNKNOWN  (0xFFFFFFFFu)
#define PINMUX_FUNC_SEL_BITS_UNKNOWN    (0xFFFFFFFFu)

/* API */
int32_t gpio_pinmux_init(const struct gpio_core_ops* ops);
int32_t gpio_pinmux_config(uint32_t pin, uint32_t function_sel);
int32_t gpio_pinmux_get(uint32_t pin, uint32_t* function_sel_out);

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_DSP_PIN_MUX_CONTROL_H */
