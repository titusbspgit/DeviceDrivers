/*
 * TMS320C6452 DSP - Pin multiplexing control - Feature Driver (Source)
 * Register map is UNKNOWN in this context; provide stubs that return -ENOTSUP.
 */
#include <stdint.h>
#include "tms320c6452_dsp_pin_multiplexing_control.h"

int tms320c6452_dsp_pinmux_validate_ready(void)
{
    (void)tms320c6452_core_get_ops(); /* dependency reference to core */
    return TMS320C6452_DSP_ENOTSUP;
}

int tms320c6452_dsp_pinmux_set_gpio_mode(u32 pin)
{
    (void)pin;
    (void)tms320c6452_core_get_ops(); /* dependency reference to core */
    return TMS320C6452_DSP_ENOTSUP;
}

int tms320c6452_dsp_pinmux_get_mode(u32 pin, u32 *mode)
{
    (void)pin; (void)mode;
    (void)tms320c6452_core_get_ops(); /* dependency reference to core */
    return TMS320C6452_DSP_ENOTSUP;
}
