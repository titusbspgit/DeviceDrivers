#ifndef TMS320C6452_DSP_PIN_MULTIPLEXING_CONTROL_H
#define TMS320C6452_DSP_PIN_MULTIPLEXING_CONTROL_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* NOTE: Device-level PINMUX register map is not provided here. */
#define TMS320C6452_DSP_PINMUX_REGISTERS_STATUS_UNKNOWN 1

/* Status codes */
#define TMS320C6452_DSP_PINMUX_OK             (0)
#define TMS320C6452_DSP_PINMUX_ERR_UNKNOWN   (-2)

/* Generic request object for selecting GPIO function on a given pin. */
typedef struct {
    uint32_t pin_id;     /* Package pin or GPIO instance number */
    uint32_t function;   /* Function selector value for GPIO (device-specific) */
} tms320c6452_dsp_pinmux_request_t;

/* Initialize pin multiplexing control (no-op without concrete register map). */
int  tms320c6452_dsp_pin_multiplexing_control_init(void);

/* Indicate whether concrete PINMUX registers are known at compile-time. */
bool tms320c6452_dsp_pin_multiplexing_control_is_ready(void);

/* Apply a pinmux request. Returns ERR_UNKNOWN when register map is unavailable. */
int  tms320c6452_dsp_pin_multiplexing_control_apply(const tms320c6452_dsp_pinmux_request_t *req);

/* Read back configuration for a pin. Returns ERR_UNKNOWN when unavailable. */
int  tms320c6452_dsp_pin_multiplexing_control_readback(uint32_t pin_id, uint32_t *out_function);

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_DSP_PIN_MULTIPLEXING_CONTROL_H */
