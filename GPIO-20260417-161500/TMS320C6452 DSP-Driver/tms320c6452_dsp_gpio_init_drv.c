/*
 * TMS320C6452 DSP - GPIO_INIT feature driver (source)
 * Initializes GPIO to safe default states using core ops.
 */
#include <stdint.h>
#include "tms320c6452_dsp_gpio_init.h"

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

int tms320c6452_dsp_gpio_init(void)
{
    /* Disable bank interrupts */
    wr32(GPIO_BINTEN_OFF, 0x00000000u);

    /* Disable edges */
    wr32(GPIO_CLR_RIS_TRIG01_OFF, 0xFFFFFFFFu);
    wr32(GPIO_CLR_FAL_TRIG01_OFF, 0xFFFFFFFFu);

    /* Clear pending */
    wr32(GPIO_INTSTAT01_OFF, 0xFFFFFFFFu);

    /* All inputs */
    wr32(GPIO_DIR01_OFF, 0xFFFFFFFFu);

    /* Verify */
    if (rd32(GPIO_BINTEN_OFF) != 0x00000000u) return -1;
    if (rd32(GPIO_DIR01_OFF)  != 0xFFFFFFFFu) return -2;
    if (rd32(GPIO_SET_RIS_TRIG01_OFF) != 0x00000000u) return -3;
    if (rd32(GPIO_SET_FAL_TRIG01_OFF) != 0x00000000u) return -4;
    if (rd32(GPIO_INTSTAT01_OFF) != 0x00000000u) return -5;

    return 0;
}

uint32_t tms320c6452_dsp_gpio_read_dir(void)     { return rd32(GPIO_DIR01_OFF); }
uint32_t tms320c6452_dsp_gpio_read_in(void)      { return rd32(GPIO_IN_DATA01_OFF); }
uint32_t tms320c6452_dsp_gpio_read_binten(void)  { return rd32(GPIO_BINTEN_OFF); }
uint32_t tms320c6452_dsp_gpio_read_rise_en(void) { return rd32(GPIO_SET_RIS_TRIG01_OFF); }
uint32_t tms320c6452_dsp_gpio_read_fall_en(void) { return rd32(GPIO_SET_FAL_TRIG01_OFF); }
uint32_t tms320c6452_dsp_gpio_read_intstat(void) { return rd32(GPIO_INTSTAT01_OFF); }

void tms320c6452_dsp_gpio_write_output_mask(uint32_t set_mask, uint32_t clr_mask)
{
    if (set_mask) wr32(GPIO_SET_DATA01_OFF, set_mask);
    if (clr_mask) wr32(GPIO_CLR_DATA01_OFF, clr_mask);
}
