/* tms320c6452_dsp_per_pin_direct_interrupt_option.h - Per-pin direct interrupt option driver (SINGLE FEATURE)
 * Constraints: Memory-mapped I/O, no dynamic alloc, deterministic access
 * Missing register data: UNKNOWN (device-specific mapping not available in source CSV)
 */
#ifndef TMS320C6452_DSP_PER_PIN_DIRECT_INTERRUPT_OPTION_H
#define TMS320C6452_DSP_PER_PIN_DIRECT_INTERRUPT_OPTION_H

#include <stdint.h>
#include <stdbool.h>
#include "core_TMS320C6452_DSP.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Register definitions: UNKNOWN */
/* Bitfields: UNKNOWN */

/* Error codes */
#define DRV_OK            (0)
#define DRV_INVALID_ARG   (-22)
#define DRV_UNSUPPORTED   (-2)

/* Configuration structure for mapping a GPIO pin directly to a CPU interrupt vector */
typedef struct {
    uint32_t pin;        /* GPIO pin index */
    uint32_t cpu_int;    /* CPU interrupt vector/index */
    bool     enable;     /* true: enable mapping; false: disable */
} tms320c6452_dsp_per_pin_direct_irq_cfg_t;

/* Initialize the feature. Returns DRV_OK or DRV_UNSUPPORTED when mapping is UNKNOWN. */
int tms320c6452_dsp_per_pin_direct_interrupt_option_init(const core_tms320c6452_ops_t *ops);

/* Map a single pin to a CPU interrupt vector. Returns DRV_UNSUPPORTED when mapping registers are UNKNOWN. */
int tms320c6452_dsp_per_pin_direct_interrupt_option_map(uint32_t pin, uint32_t cpu_int);

/* Enable/disable direct interrupt for a pin. Returns DRV_UNSUPPORTED when mapping is UNKNOWN. */
int tms320c6452_dsp_per_pin_direct_interrupt_option_enable(uint32_t pin, bool enable);

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_DSP_PER_PIN_DIRECT_INTERRUPT_OPTION_H */
