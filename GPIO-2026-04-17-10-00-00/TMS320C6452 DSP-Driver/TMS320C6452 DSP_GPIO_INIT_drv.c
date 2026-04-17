/*
 * GPIO_INIT feature driver source for TMS320C6452 DSP
 * Initializes GPIO to safe default states.
 * Constraints: Memory-mapped I/O, Baremetal, gcc, big-endian
 */
#include "TMS320C6452 DSP_core_drv.h"
#include "TMS320C6452 DSP_GPIO_INIT.h"

/* Local access to core ops */
static const tms320c6452_core_ops_t *gpio_core_ops(void)
{
    return tms320c6452_core_get_ops();
}

/* Internal helpers using core ops and computed offsets */
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

int tms320c6452_dsp_gpio_init(void)
{
    const tms320c6452_core_ops_t *ops = gpio_core_ops();
    if (!ops)
        return -1;

    /* Disable bank-level interrupt exports */
    gpio_wr32(GPIO_BINTEN_OFF, 0x00000000u);

    /* Disable all rising/falling edge detection */
    gpio_wr32(GPIO_CLR_RIS_TRIG01_OFF, 0xFFFFFFFFu);
    gpio_wr32(GPIO_CLR_FAL_TRIG01_OFF, 0xFFFFFFFFu);

    /* Clear any pending interrupt status */
    gpio_wr32(GPIO_INTSTAT01_OFF, 0xFFFFFFFFu);

    /* Program all GPIOs as inputs (safe default) */
    gpio_wr32(GPIO_DIR01_OFF, 0xFFFFFFFFu);

    /* Basic verification (deterministic) */
    if (gpio_rd32(GPIO_BINTEN_OFF) != 0x00000000u) return -2;
    if (gpio_rd32(GPIO_DIR01_OFF)  != 0xFFFFFFFFu) return -3;

    /* Rising/Falling enable readback reflects enable map; expect 0 after disable */
    if (gpio_rd32(GPIO_SET_RIS_TRIG01_OFF) != 0x00000000u) return -4;
    if (gpio_rd32(GPIO_SET_FAL_TRIG01_OFF) != 0x00000000u) return -5;

    /* Pending status should read 0 after W1C clear */
    if (gpio_rd32(GPIO_INTSTAT01_OFF) != 0x00000000u) return -6;

    return 0;
}

/* Readback helpers */
unsigned int tms320c6452_dsp_gpio_read_dir(void)
{
    return gpio_rd32(GPIO_DIR01_OFF);
}

unsigned int tms320c6452_dsp_gpio_read_in(void)
{
    return gpio_rd32(GPIO_IN_DATA01_OFF);
}

unsigned int tms320c6452_dsp_gpio_read_binten(void)
{
    return gpio_rd32(GPIO_BINTEN_OFF);
}

unsigned int tms320c6452_dsp_gpio_read_rise_en(void)
{
    return gpio_rd32(GPIO_SET_RIS_TRIG01_OFF);
}

unsigned int tms320c6452_dsp_gpio_read_fall_en(void)
{
    return gpio_rd32(GPIO_SET_FAL_TRIG01_OFF);
}

unsigned int tms320c6452_dsp_gpio_read_intstat(void)
{
    return gpio_rd32(GPIO_INTSTAT01_OFF);
}
