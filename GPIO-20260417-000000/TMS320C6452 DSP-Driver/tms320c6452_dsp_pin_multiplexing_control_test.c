/*
 * TMS320C6452 DSP - Pin multiplexing control - Feature Test
 * Expects -ENOTSUP from all APIs (register map UNKNOWN here).
 */
#include <stdint.h>
#include "tms320c6452_dsp_pin_multiplexing_control.h"

static int expect_eq_int(int a, int b) { return (a == b) ? 0 : -1; }

int main(void)
{
    int rc;

    rc = tms320c6452_dsp_pinmux_validate_ready();
    if (expect_eq_int(rc, TMS320C6452_DSP_ENOTSUP)) return -10;

    rc = tms320c6452_dsp_pinmux_set_gpio_mode(0u);
    if (expect_eq_int(rc, TMS320C6452_DSP_ENOTSUP)) return -11;

    rc = tms320c6452_dsp_pinmux_get_mode(0u, (unsigned int *)0);
    if (expect_eq_int(rc, TMS320C6452_DSP_ENOTSUP)) return -12;

    return 0;
}
