/*
 * TMS320C6452 DSP - GPIO_INIT feature test
 */
#include <stdint.h>
#include "tms320c6452_dsp_gpio_init.h"

static int expect_eq_u32(uint32_t got, uint32_t exp) { return (got == exp) ? 0 : -1; }

int main(void)
{
    int rc;
    uint32_t v;

    rc = tms320c6452_dsp_gpio_init();
    if (rc != 0) return rc;

    v = tms320c6452_dsp_gpio_read_binten();
    if (expect_eq_u32(v, 0x00000000u)) return -10;

    v = tms320c6452_dsp_gpio_read_dir();
    if (expect_eq_u32(v, 0xFFFFFFFFu)) return -11;

    v = tms320c6452_dsp_gpio_read_rise_en();
    if (expect_eq_u32(v, 0x00000000u)) return -12;

    v = tms320c6452_dsp_gpio_read_fall_en();
    if (expect_eq_u32(v, 0x00000000u)) return -13;

    /* Clear and verify */
    tms320c6452_dsp_gpio_write_output_mask(0,0); /* no-op; placeholder */
    v = tms320c6452_dsp_gpio_read_intstat();
    if (expect_eq_u32(v, 0x00000000u)) return -14;

    (void)tms320c6452_dsp_gpio_read_in();

    return 0;
}
