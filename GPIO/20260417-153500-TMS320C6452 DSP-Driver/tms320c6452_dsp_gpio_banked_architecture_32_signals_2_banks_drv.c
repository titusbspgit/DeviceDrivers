#include "tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks.h"

static inline void wr32(uint32_t off, uint32_t v) { GPIO_REG32(off) = v; }
static inline uint32_t rd32(uint32_t off) { return GPIO_REG32(off); }

void tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_init(void)
{
    /* Clear pending interrupts for a clean start */
    wr32(GPIO_INTSTAT01_OFFSET, 0xFFFFFFFFu);
}

bool tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_set_direction(uint32_t pin, bool input_mode)
{
    if (!GPIO_VALID_PIN(pin)) return false;
    uint32_t dir = rd32(GPIO_DIR01_OFFSET);
    if (input_mode) dir |= GPIO_BIT(pin); else dir &= ~GPIO_BIT(pin);
    wr32(GPIO_DIR01_OFFSET, dir);
    return true;
}

bool tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_write(uint32_t pin, bool level)
{
    if (!GPIO_VALID_PIN(pin)) return false;
    if (level) wr32(GPIO_SET_DATA01_OFFSET, GPIO_BIT(pin));
    else       wr32(GPIO_CLR_DATA01_OFFSET, GPIO_BIT(pin));
    return true;
}

bool tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_set(uint32_t pin)
{
    if (!GPIO_VALID_PIN(pin)) return false;
    wr32(GPIO_SET_DATA01_OFFSET, GPIO_BIT(pin));
    return true;
}

bool tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_clear(uint32_t pin)
{
    if (!GPIO_VALID_PIN(pin)) return false;
    wr32(GPIO_CLR_DATA01_OFFSET, GPIO_BIT(pin));
    return true;
}

bool tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_toggle(uint32_t pin)
{
    if (!GPIO_VALID_PIN(pin)) return false;
    uint32_t out = rd32(GPIO_OUT_DATA01_OFFSET);
    if ((out & GPIO_BIT(pin)) != 0u) wr32(GPIO_CLR_DATA01_OFFSET, GPIO_BIT(pin));
    else                             wr32(GPIO_SET_DATA01_OFFSET, GPIO_BIT(pin));
    return true;
}

int tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_read_in(uint32_t pin)
{
    if (!GPIO_VALID_PIN(pin)) return -1;
    uint32_t v = rd32(GPIO_IN_DATA01_OFFSET);
    return (v & GPIO_BIT(pin)) ? 1 : 0;
}

int tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_read_out(uint32_t pin)
{
    if (!GPIO_VALID_PIN(pin)) return -1;
    uint32_t v = rd32(GPIO_OUT_DATA01_OFFSET);
    return (v & GPIO_BIT(pin)) ? 1 : 0;
}

bool tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_write_mask(uint32_t set_mask, uint32_t clr_mask)
{
    wr32(GPIO_SET_DATA01_OFFSET, set_mask);
    wr32(GPIO_CLR_DATA01_OFFSET, clr_mask);
    return true;
}

bool tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_irq_enable(uint32_t pin, bool rising, bool falling)
{
    if (!GPIO_VALID_PIN(pin)) return false;
    if (rising)  wr32(GPIO_SET_RIS_TRIG01_OFFSET, GPIO_BIT(pin));
    else         wr32(GPIO_CLR_RIS_TRIG01_OFFSET, GPIO_BIT(pin));
    if (falling)  wr32(GPIO_SET_FAL_TRIG01_OFFSET, GPIO_BIT(pin));
    else          wr32(GPIO_CLR_FAL_TRIG01_OFFSET, GPIO_BIT(pin));
    return true;
}

bool tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_irq_disable(uint32_t pin)
{
    if (!GPIO_VALID_PIN(pin)) return false;
    wr32(GPIO_CLR_RIS_TRIG01_OFFSET, GPIO_BIT(pin));
    wr32(GPIO_CLR_FAL_TRIG01_OFFSET, GPIO_BIT(pin));
    return true;
}

uint32_t tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_irq_status(void)
{
    return rd32(GPIO_INTSTAT01_OFFSET);
}

void tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_irq_clear(uint32_t mask)
{
    wr32(GPIO_INTSTAT01_OFFSET, mask);
}

bool tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_bank_irq_gate(uint32_t bank, bool enable)
{
    if (bank > 1u) return false;
    uint32_t v = rd32(GPIO_BINTEN_OFFSET);
    if (enable) v |= (1u << bank); else v &= ~(1u << bank);
    wr32(GPIO_BINTEN_OFFSET, v);
    return true;
}
