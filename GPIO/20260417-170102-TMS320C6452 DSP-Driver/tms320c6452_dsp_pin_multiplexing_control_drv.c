#include "tms320c6452_dsp_pin_multiplexing_control.h"

int tms320c6452_dsp_pin_multiplexing_control_init(void)
{
    /* Without concrete device PINMUX register map, perform no hardware writes. */
    return TMS320C6452_DSP_PINMUX_OK;
}

bool tms320c6452_dsp_pin_multiplexing_control_is_ready(void)
{
    /* Register addresses and bitfields are UNKNOWN in this context. */
    return false;
}

int tms320c6452_dsp_pin_multiplexing_control_apply(const tms320c6452_dsp_pinmux_request_t *req)
{
    (void)req;
    /* Missing data -> UNKNOWN */
    return TMS320C6452_DSP_PINMUX_ERR_UNKNOWN;
}

int tms320c6452_dsp_pin_multiplexing_control_readback(uint32_t pin_id, uint32_t *out_function)
{
    (void)pin_id;
    if (out_function) { *out_function = 0u; }
    /* Missing data -> UNKNOWN */
    return TMS320C6452_DSP_PINMUX_ERR_UNKNOWN;
}
