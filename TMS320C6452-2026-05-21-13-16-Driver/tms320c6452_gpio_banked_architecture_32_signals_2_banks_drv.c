#include "tms320c6452_gpio_banked_architecture_32_signals_2_banks.h"

static tms320c6452_gpio_irq_cb_t s_gpio_cb[32] = {0};

static inline bool pin_valid(uint32_t pin) { return (pin < 32u); }
static inline bool bank_valid(uint32_t bank) { return (bank < 2u); }

void tms320c6452_gpio_banked_init(void)
{
    tms320c6452_gpio_core.write32(TMS320C6452_GPIO_INTSTAT01_OFS, 0xFFFFFFFFu);
}

int tms320c6452_gpio_banked_set_direction(uint32_t pin, bool input)
{
    if (!pin_valid(pin)) return TMS320C6452_GPIO_EINVAL;
    uint32_t v = tms320c6452_gpio_core.read32(TMS320C6452_GPIO_DIR01_OFS);
    if (input) {
        v |= TMS320C6452_GPIO_PIN_MASK(pin);
    } else {
        v &= ~TMS320C6452_GPIO_PIN_MASK(pin);
    }
    tms320c6452_gpio_core.write32(TMS320C6452_GPIO_DIR01_OFS, v);
    return TMS320C6452_GPIO_OK;
}

int tms320c6452_gpio_banked_get_direction(uint32_t pin, bool *input)
{
    if (!pin_valid(pin) || (input == (bool*)0)) return TMS320C6452_GPIO_EINVAL;
    uint32_t v = tms320c6452_gpio_core.read32(TMS320C6452_GPIO_DIR01_OFS);
    *input = ((v & TMS320C6452_GPIO_PIN_MASK(pin)) != 0u);
    return TMS320C6452_GPIO_OK;
}

int tms320c6452_gpio_banked_set_pin(uint32_t pin)
{
    if (!pin_valid(pin)) return TMS320C6452_GPIO_EINVAL;
    tms320c6452_gpio_core.write32(TMS320C6452_GPIO_SET_DATA01_OFS, TMS320C6452_GPIO_PIN_MASK(pin));
    return TMS320C6452_GPIO_OK;
}

int tms320c6452_gpio_banked_clear_pin(uint32_t pin)
{
    if (!pin_valid(pin)) return TMS320C6452_GPIO_EINVAL;
    tms320c6452_gpio_core.write32(TMS320C6452_GPIO_CLR_DATA01_OFS, TMS320C6452_GPIO_PIN_MASK(pin));
    return TMS320C6452_GPIO_OK;
}

int tms320c6452_gpio_banked_write_pin(uint32_t pin, uint32_t value)
{
    if (!pin_valid(pin)) return TMS320C6452_GPIO_EINVAL;
    if (value != 0u) {
        tms320c6452_gpio_core.write32(TMS320C6452_GPIO_SET_DATA01_OFS, TMS320C6452_GPIO_PIN_MASK(pin));
    } else {
        tms320c6452_gpio_core.write32(TMS320C6452_GPIO_CLR_DATA01_OFS, TMS320C6452_GPIO_PIN_MASK(pin));
    }
    return TMS320C6452_GPIO_OK;
}

int tms320c6452_gpio_banked_read_pin(uint32_t pin, uint32_t *value)
{
    if (!pin_valid(pin) || (value == (uint32_t*)0)) return TMS320C6452_GPIO_EINVAL;
    uint32_t v = tms320c6452_gpio_core.read32(TMS320C6452_GPIO_IN_DATA01_OFS);
    *value = ((v & TMS320C6452_GPIO_PIN_MASK(pin)) != 0u) ? 1u : 0u;
    return TMS320C6452_GPIO_OK;
}

int tms320c6452_gpio_banked_read_bank_inputs(uint32_t bank, uint16_t *value16)
{
    if (!bank_valid(bank) || (value16 == (uint16_t*)0)) return TMS320C6452_GPIO_EINVAL;
    uint32_t v = tms320c6452_gpio_core.read32(TMS320C6452_GPIO_IN_DATA01_OFS);
    if (bank == 0u) {
        *value16 = (uint16_t)(v & 0xFFFFu);
    } else {
        *value16 = (uint16_t)((v >> 16) & 0xFFFFu);
    }
    return TMS320C6452_GPIO_OK;
}

int tms320c6452_gpio_banked_write_bank_mask(uint32_t bank, uint16_t set_mask, uint16_t clear_mask)
{
    if (!bank_valid(bank)) return TMS320C6452_GPIO_EINVAL;

    uint32_t set_val = (bank == 0u) ? (uint32_t)set_mask : ((uint32_t)set_mask << 16);
    uint32_t clr_val = (bank == 0u) ? (uint32_t)clear_mask : ((uint32_t)clear_mask << 16);

    if (set_val != 0u) {
        tms320c6452_gpio_core.write32(TMS320C6452_GPIO_SET_DATA01_OFS, set_val);
    }
    if (clr_val != 0u) {
        tms320c6452_gpio_core.write32(TMS320C6452_GPIO_CLR_DATA01_OFS, clr_val);
    }
    return TMS320C6452_GPIO_OK;
}

int tms320c6452_gpio_banked_enable_bank_irq(uint32_t bank, bool enable)
{
    if (!bank_valid(bank)) return TMS320C6452_GPIO_EINVAL;
    uint32_t m = TMS320C6452_GPIO_BANK_MASK(bank);
    uint32_t v = tms320c6452_gpio_core.read32(TMS320C6452_GPIO_BINTEN_OFS);
    if (enable) v |= m; else v &= ~m;
    tms320c6452_gpio_core.write32(TMS320C6452_GPIO_BINTEN_OFS, v);
    return TMS320C6452_GPIO_OK;
}

int tms320c6452_gpio_banked_configure_pin_irq(uint32_t pin, bool rising, bool falling)
{
    if (!pin_valid(pin)) return TMS320C6452_GPIO_EINVAL;
    uint32_t m = TMS320C6452_GPIO_PIN_MASK(pin);

    if (rising) {
        tms320c6452_gpio_core.set_bits(TMS320C6452_GPIO_SET_RIS_TRIG01_OFS, m);
    } else {
        tms320c6452_gpio_core.write32(TMS320C6452_GPIO_CLR_RIS_TRIG01_OFS, m);
    }

    if (falling) {
        tms320c6452_gpio_core.set_bits(TMS320C6452_GPIO_SET_FAL_TRIG01_OFS, m);
    } else {
        tms320c6452_gpio_core.write32(TMS320C6452_GPIO_CLR_FAL_TRIG01_OFS, m);
    }

    tms320c6452_gpio_core.write32(TMS320C6452_GPIO_INTSTAT01_OFS, m);
    return TMS320C6452_GPIO_OK;
}

uint32_t tms320c6452_gpio_banked_get_pending(void)
{
    return tms320c6452_gpio_core.read32(TMS320C6452_GPIO_INTSTAT01_OFS);
}

void tms320c6452_gpio_banked_clear_pending(uint32_t mask)
{
    if (mask != 0u) {
        tms320c6452_gpio_core.write32(TMS320C6452_GPIO_INTSTAT01_OFS, mask);
    }
}

int tms320c6452_gpio_banked_register_callback(uint32_t pin, tms320c6452_gpio_irq_cb_t cb)
{
    if (!pin_valid(pin)) return TMS320C6452_GPIO_EINVAL;
    s_gpio_cb[pin] = cb;
    return TMS320C6452_GPIO_OK;
}

void tms320c6452_gpio_banked_isr(void)
{
    uint32_t pend = tms320c6452_gpio_core.read32(TMS320C6452_GPIO_INTSTAT01_OFS);
    if (pend == 0u) {
        return;
    }
    tms320c6452_gpio_core.write32(TMS320C6452_GPIO_INTSTAT01_OFS, pend);
    for (uint32_t pin = 0u; pin < 32u; ++pin) {
        if ((pend & TMS320C6452_GPIO_PIN_MASK(pin)) != 0u) {
            tms320c6452_gpio_irq_cb_t cb = s_gpio_cb[pin];
            if (cb) {
                cb(pin);
            }
        }
    }
}
