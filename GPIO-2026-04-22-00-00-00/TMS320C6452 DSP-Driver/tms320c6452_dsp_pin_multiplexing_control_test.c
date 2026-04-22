/*
 * Test: Pin multiplexing control
 */
#include <stdio.h>
#include "tms320c6452_dsp_pin_multiplexing_control.h"

int main(void)
{
    gpio_drv_status_t st;
    st = gpio_pinmux_validate(0u);
    printf("pinmux_validate(0): status=%d (expected unsupported)\n", (int)st);
    st = gpio_pinmux_configure(0u, 0u);
    printf("pinmux_configure(0,0): status=%d (expected unsupported)\n", (int)st);
    return 0;
}
