/*
 * TMS320C6452 DSP - GPIO_IRQ feature driver (source)
 * Provides edge-trigger interrupt configuration and status handling using core ops.
 */
#include <stdint.h>
#include "tms320c6452_dsp_gpio_irq.h"

static inline const tms320c6452_core_ops_t *ops(void)
{
    return tms320c6452_core_get_ops();
}

static inline void wr32(uint32_t reg_off, uint32_t v)
{
    ops()->write32(TMS320C6452_DSP_GPIO_OFF(reg_off), v);
}

static inline uint32_t rd32(uint32_t reg_off)
{
    return ops()->read32(TMS320C6452_DSP_GPIO_OFF(reg_off));
}

int tms320c6452_dsp_gpio_irq_init(uint32_t bank_enable_mask)
{
    /* Program BINTEN to enable bank-level interrupt export */
    wr32(GPIO_BINTEN_OFF, bank_enable_mask & (BIT(0) | BIT(1)));

    if (rd32(GPIO_BINTEN_OFF) != (bank_enable_mask & (BIT(0) | BIT(1))))
        return -1;

    /* Clear any stale pending status */
    wr32(GPIO_INTSTAT01_OFF, 0xFFFFFFFFu);
    if (rd32(GPIO_INTSTAT01_OFF) != 0x00000000u) return -2;

    return 0;
}

void tms320c6452_dsp_gpio_irq_enable_rising(uint32_t pins_mask)
{
    wr32(GPIO_SET_RIS_TRIG01_OFF, pins_mask);
}

void tms320c6452_dsp_gpio_irq_disable_rising(uint32_t pins_mask)
{
    wr32(GPIO_CLR_RIS_TRIG01_OFF, pins_mask);
}

void tms320c6452_dsp_gpio_irq_enable_falling(uint32_t pins_mask)
{
    wr32(GPIO_SET_FAL_TRIG01_OFF, pins_mask);
}

void tms320c6452_dsp_gpio_irq_disable_falling(uint32_t pins_mask)
{
    wr32(GPIO_CLR_FAL_TRIG01_OFF, pins_mask);
}

uint32_t tms320c6452_dsp_gpio_irq_status(void)
{
    return rd32(GPIO_INTSTAT01_OFF);
}

void tms320c6452_dsp_gpio_irq_clear(uint32_t pins_mask)
{
    wr32(GPIO_INTSTAT01_OFF, pins_mask);
    (void)rd32(GPIO_INTSTAT01_OFF);
}

uint32_t tms320c6452_dsp_gpio_irq_rise_en(void)
{
    return rd32(GPIO_SET_RIS_TRIG01_OFF);
}

uint32_t tms320c6452_dsp_gpio_irq_fall_en(void)
{
    return rd32(GPIO_SET_FAL_TRIG01_OFF);
}
