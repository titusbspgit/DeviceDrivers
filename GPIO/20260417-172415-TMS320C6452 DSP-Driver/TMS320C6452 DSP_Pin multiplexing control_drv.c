#include "TMS320C6452 DSP_Pin multiplexing control.h"

int TMS320C6452_DSP_Pin_multiplexing_control_init(void)
{
    return TMS320C6452_DSP_PINMUX_OK;
}

bool TMS320C6452_DSP_Pin_multiplexing_control_is_ready(void)
{
    return false;
}

int TMS320C6452_DSP_Pin_multiplexing_control_apply(const TMS320C6452_DSP_pinmux_request_t *req)
{
    (void)req;
    return TMS320C6452_DSP_PINMUX_ERR_UNKNOWN;
}

int TMS320C6452_DSP_Pin_multiplexing_control_readback(uint32_t pin_id, uint32_t *out_function)
{
    (void)pin_id;
    if (out_function) { *out_function = 0u; }
    return TMS320C6452_DSP_PINMUX_ERR_UNKNOWN;
}
