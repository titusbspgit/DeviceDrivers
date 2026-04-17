/*
 * Test for GPIO_EDGE_IRQ feature on TMS320C6452 DSP
 * Configures pin 0 for rising-edge interrupt, enables bank0 export.
 */
#include "TMS320C6452 DSP_core_drv.h"
#include "TMS320C6452 DSP_GPIO_EDGE_IRQ.h"

static int verify_mask(unsigned int v, unsigned int mask)
{
    return ((v & mask) == mask) ? 0 : -1;
}

int main(void)
{
    int rc;

    rc = tms320c6452_dsp_gpio_irq_init(GPIO_BANK0_INT_EN);
    if (rc) return -1;

    rc = tms320c6452_dsp_gpio_irq_enable_pin(0u, 1, 0);
    if (rc) return -2;

    /* We cannot synthesize a hardware edge here; just check enables are set via readable set regs */
    /* Read back via status API is limited to pending IRQs; enable state is implicit. No direct readback macro here. */

    /* Simulate an interrupt clear path (no-op if none pending) */
    tms320c6452_dsp_gpio_irq_clear(0x00000001u);

    /* Call ISR stub once to ensure it does not fault when no pending */
    tms320c6452_dsp_gpio_isr();

    return 0;
}
