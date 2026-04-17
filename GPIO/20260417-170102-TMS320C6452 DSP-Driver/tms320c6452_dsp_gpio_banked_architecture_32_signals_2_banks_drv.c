#include "tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks.h"

static inline void wr32(uint32_t off, uint32_t v)
{
    *(volatile uint32_t *)(TMS320C6452_DSP_GPIO_BASE + (uintptr_t)off) = v;
}

static inline uint32_t rd32(uint32_t off)
{
    return *(volatile uint32_t *)(TMS320C6452_DSP_GPIO_BASE + (uintptr_t)off);
}

void tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_init(void)
{
    /* Non-intrusive initialization: clear any pending interrupts; leave DIR/OUT as-is */
    wr32(GPIO_INTSTAT01_OFFSET, 0xFFFFFFFFu); /* W1C */
}

bool tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_set_dir_mask(uint32_t input_mask)
{
    /* Each bit: 1=input, 0=output */
    wr32(GPIO_DIR01_OFFSET, input_mask);
    return true;
}

bool tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_write_mask(uint32_t set_mask, uint32_t clear_mask)
{
    wr32(GPIO_SET_DATA01_OFFSET, set_mask);
    wr32(GPIO_CLR_DATA01_OFFSET, clear_mask);
    return true;
}

static inline uint16_t slice_bank(uint32_t v, unsigned bank)
{
    return (bank == 0u) ? (uint16_t)(v & 0xFFFFu) : (uint16_t)((v >> 16) & 0xFFFFu);
}

uint16_t tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_read_in_bank(unsigned bank)
{
    if (bank > 1u) return 0u;
    uint32_t v = rd32(GPIO_IN_DATA01_OFFSET);
    return slice_bank(v, bank);
}

uint16_t tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_read_out_bank(unsigned bank)
{
    if (bank > 1u) return 0u;
    uint32_t v = rd32(GPIO_OUT_DATA01_OFFSET);
    return slice_bank(v, bank);
}

bool tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_enable_bank_interrupt(unsigned bank, bool enable)
{
    if (bank > 1u) return false;
    uint32_t binten = rd32(GPIO_BINTEN_OFFSET);
    uint32_t bit = (bank == 0u) ? 0u : 1u;
    if (enable) binten |= (1u << bit); else binten &= ~(1u << bit);
    wr32(GPIO_BINTEN_OFFSET, binten);
    return true;
}

bool tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_configure_pin_edge(unsigned pin, bool rise_en, bool fall_en)
{
    if (pin > 31u) return false;
    uint32_t m = GPIO_BIT(pin);
    if (rise_en) { wr32(GPIO_SET_RIS_TRIG01_OFFSET, m); } else { wr32(GPIO_CLR_RIS_TRIG01_OFFSET, m); }
    if (fall_en) { wr32(GPIO_SET_FAL_TRIG01_OFFSET, m); } else { wr32(GPIO_CLR_FAL_TRIG01_OFFSET, m); }
    return true;
}

uint32_t tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_get_int_status(void)
{
    return rd32(GPIO_INTSTAT01_OFFSET);
}

void tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_clear_int_status(uint32_t mask)
{
    /* W1C */
    wr32(GPIO_INTSTAT01_OFFSET, mask);
}
