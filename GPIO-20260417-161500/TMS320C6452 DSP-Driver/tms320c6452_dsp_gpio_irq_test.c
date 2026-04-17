/*
 * TMS320C6452 DSP - GPIO_IRQ feature test
 */
#include <stdint.h>
#include "tms320c6452_dsp_gpio_irq.h"

static int expect_eq_u32(uint32_t got, uint32_t exp) { return (got == exp) ? 0 : -1; }

int main(void)
{
    int rc;
    uint32_t v;
    const uint32_t test_mask = (BIT(0) | BIT(16));

    rc = tms320c6452_dsp_gpio_irq_init(BIT(0) | BIT(1));
    if (rc) return -10;

    tms320c6452_dsp_gpio_irq_enable_rising(test_mask);
    tms320c6452_dsp_gpio_irq_enable_falling(test_mask);

    v = tms320c6452_dsp_gpio_irq_rise_en();
    if (expect_eq_u32(v & test_mask, test_mask)) return -11;

    v = tms320c6452_dsp_gpio_irq_fall_en();
    if (expect_eq_u32(v & test_mask, test_mask)) return -12;

    tms320c6452_dsp_gpio_irq_clear(0xFFFFFFFFu);
    v = tms320c6452_dsp_gpio_irq_status();
    if (expect_eq_u32(v, 0x00000000u)) return -13;

    tms320c6452_dsp_gpio_irq_disable_rising(test_mask);
    tms320c6452_dsp_gpio_irq_disable_falling(test_mask);

    v = tms320c6452_dsp_gpio_irq_rise_en();
    if (expect_eq_u32(v & test_mask, 0x00000000u)) return -14;

    v = tms320c6452_dsp_gpio_irq_fall_en();
    if (expect_eq_u32(v & test_mask, 0x00000000u)) return -15;

    return 0;
}
