/*
 * Test application for GPIO_INIT feature on TMS320C6452 DSP
 * Sequence: init -> verify registers -> edge cases
 * Baremetal, gcc, big-endian
 */
#include "TMS320C6452 DSP_core_drv.h"
#include "TMS320C6452 DSP_GPIO_INIT.h"

static int verify_equal_u32(unsigned int got, unsigned int exp)
{
    return (got == exp) ? 0 : -1;
}

int main(void)
{
    int rc;
    unsigned int v;

    rc = tms320c6452_dsp_gpio_init();
    if (rc != 0) return rc;

    /* Verify BINTEN disabled */
    v = tms320c6452_dsp_gpio_read_binten();
    if (verify_equal_u32(v, 0x00000000u)) return -10;

    /* Verify DIR = all inputs */
    v = tms320c6452_dsp_gpio_read_dir();
    if (verify_equal_u32(v, 0xFFFFFFFFu)) return -11;

    /* Verify rise/fall enables disabled */
    v = tms320c6452_dsp_gpio_read_rise_en();
    if (verify_equal_u32(v, 0x00000000u)) return -12;

    v = tms320c6452_dsp_gpio_read_fall_en();
    if (verify_equal_u32(v, 0x00000000u)) return -13;

    /* Clear and verify no pending status */
    {
        const tms320c6452_core_ops_t *ops = tms320c6452_core_get_ops();
        ops->write32(TMS320C6452_DSP_GPIO_OFF(GPIO_INTSTAT01_OFF), 0xFFFFFFFFu);
    }
    v = tms320c6452_dsp_gpio_read_intstat();
    if (verify_equal_u32(v, 0x00000000u)) return -14;

    /* Edge case: read input state */
    (void)tms320c6452_dsp_gpio_read_in();

    return 0;
}
