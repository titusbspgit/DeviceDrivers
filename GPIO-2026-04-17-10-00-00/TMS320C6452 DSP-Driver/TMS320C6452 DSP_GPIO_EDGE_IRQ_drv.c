/*
 * GPIO_EDGE_IRQ feature driver for TMS320C6452 DSP
 * Configure and service GPIO edge-triggered interrupts.
 */
#include "TMS320C6452 DSP_core_drv.h"
#include "TMS320C6452 DSP_GPIO_EDGE_IRQ.h"

static const tms320c6452_core_ops_t *gpio_core_ops(void)
{
    return tms320c6452_core_get_ops();
}

static inline void gpio_wr32(u32 reg_off, u32 val)
{
    const tms320c6452_core_ops_t *ops = gpio_core_ops();
    ops->write32(TMS320C6452_DSP_GPIO_OFF(reg_off), val);
}

static inline u32 gpio_rd32(u32 reg_off)
{
    const tms320c6452_core_ops_t *ops = gpio_core_ops();
    return ops->read32(TMS320C6452_DSP_GPIO_OFF(reg_off));
}

int tms320c6452_dsp_gpio_irq_init(unsigned int bank_mask)
{
    /* Disable all per-pin detectors */
    gpio_wr32(GPIO_CLR_RIS_TRIG01_OFF, 0xFFFFFFFFu);
    gpio_wr32(GPIO_CLR_FAL_TRIG01_OFF, 0xFFFFFFFFu);

    /* Clear any pending interrupts */
    gpio_wr32(GPIO_INTSTAT01_OFF, 0xFFFFFFFFu);

    /* Enable bank interrupt export per mask (bits 0: bank0, 1: bank1) */
    gpio_wr32(GPIO_BINTEN_OFF, bank_mask & (GPIO_BANK0_INT_EN | GPIO_BANK1_INT_EN));

    return 0;
}

int tms320c6452_dsp_gpio_irq_enable_pin(unsigned int pin, int rising, int falling)
{
    if (pin > 31u) return -1;

    /* Ensure pin is input for edge detection */
    /* DIR bit = 1 for input; set corresponding bit */
    u32 dir = gpio_rd32(GPIO_DIR01_OFF);
    dir |= GPIO_PIN(pin);
    gpio_wr32(GPIO_DIR01_OFF, dir);

    if (rising)
        gpio_wr32(GPIO_SET_RIS_TRIG01_OFF, GPIO_PIN(pin));
    if (falling)
        gpio_wr32(GPIO_SET_FAL_TRIG01_OFF, GPIO_PIN(pin));

    return 0;
}

int tms320c6452_dsp_gpio_irq_disable_pin(unsigned int pin)
{
    if (pin > 31u) return -1;

    gpio_wr32(GPIO_CLR_RIS_TRIG01_OFF, GPIO_PIN(pin));
    gpio_wr32(GPIO_CLR_FAL_TRIG01_OFF, GPIO_PIN(pin));

    return 0;
}

unsigned int tms320c6452_dsp_gpio_irq_status(void)
{
    return gpio_rd32(GPIO_INTSTAT01_OFF);
}

void tms320c6452_dsp_gpio_irq_clear(unsigned int mask)
{
    /* Write-1-to-clear */
    gpio_wr32(GPIO_INTSTAT01_OFF, mask);
}

void tms320c6452_dsp_gpio_isr(void)
{
    /* Application/platform must connect this ISR to the system INTC vector (UNKNOWN mapping) */
    u32 status = gpio_rd32(GPIO_INTSTAT01_OFF);

    /* Clear all pending we read to avoid retrigger (edge) */
    if (status)
        gpio_wr32(GPIO_INTSTAT01_OFF, status);

    /* Hook: user can inspect 'status' for pin source */
    (void)status;
}
