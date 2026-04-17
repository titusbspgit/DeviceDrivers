// GPIO Basic feature driver (single feature only)
// Deterministic, no dynamic allocation; big-endian safe via core API.

#include "TMS320C6452_DSP_gpio_basic.h"

static inline bool _chk_known(uintptr_t off)
{
    return (off != (uintptr_t)TMS320C6452_DSP_REG_UNKNOWN);
}

int TMS320C6452_DSP_gpio_basic_init(const TMS320C6452_DSP_CoreAPI *core)
{
    (void)core;
    // No global HW init sequence without RAG; return OK deterministically.
    return GPIO_BASIC_OK;
}

int TMS320C6452_DSP_gpio_basic_configure(const TMS320C6452_DSP_CoreAPI *core, uint32_t pin, gpio_dir_t dir)
{
    if ((pin > 31u))
        return GPIO_BASIC_ERR_BAD_PIN;
    if (!_chk_known(GPIO_DIR_OFFSET))
        return GPIO_BASIC_ERR_UNKNOWN_REG; // UNKNOWN

    uintptr_t reg = TMS320C6452_DSP_REG(GPIO_DIR_OFFSET);
    uint32_t v = core->rd32(reg);
    if (dir == GPIO_DIR_OUTPUT) {
        v |= GPIO_PIN_MASK(pin);
    } else {
        v &= ~GPIO_PIN_MASK(pin);
    }
    core->wr32(reg, v);
    return GPIO_BASIC_OK;
}

int TMS320C6452_DSP_gpio_basic_write(const TMS320C6452_DSP_CoreAPI *core, uint32_t pin, bool high)
{
    if ((pin > 31u))
        return GPIO_BASIC_ERR_BAD_PIN;
    if (!_chk_known(GPIO_OUT_OFFSET))
        return GPIO_BASIC_ERR_UNKNOWN_REG; // UNKNOWN

    uintptr_t reg = TMS320C6452_DSP_REG(GPIO_OUT_OFFSET);
    uint32_t v = core->rd32(reg);
    if (high) {
        v |= GPIO_PIN_MASK(pin);
    } else {
        v &= ~GPIO_PIN_MASK(pin);
    }
    core->wr32(reg, v);
    return GPIO_BASIC_OK;
}

int TMS320C6452_DSP_gpio_basic_read(const TMS320C6452_DSP_CoreAPI *core, uint32_t pin, bool *value)
{
    if ((pin > 31u) || (value == (void*)0))
        return GPIO_BASIC_ERR_BAD_PIN;
    if (!_chk_known(GPIO_IN_OFFSET))
        return GPIO_BASIC_ERR_UNKNOWN_REG; // UNKNOWN

    uintptr_t reg = TMS320C6452_DSP_REG(GPIO_IN_OFFSET);
    uint32_t v = core->rd32(reg);
    *value = ( (v & GPIO_PIN_MASK(pin)) != 0u );
    return GPIO_BASIC_OK;
}

int TMS320C6452_DSP_gpio_basic_int_enable(const TMS320C6452_DSP_CoreAPI *core, uint32_t pin, bool enable)
{
    if ((pin > 31u))
        return GPIO_BASIC_ERR_BAD_PIN;
    if (!_chk_known(GPIO_INT_EN_OFFSET))
        return GPIO_BASIC_ERR_UNKNOWN_REG; // UNKNOWN

    uintptr_t reg = TMS320C6452_DSP_REG(GPIO_INT_EN_OFFSET);
    uint32_t v = core->rd32(reg);
    if (enable) {
        v |= GPIO_PIN_MASK(pin);
    } else {
        v &= ~GPIO_PIN_MASK(pin);
    }
    core->wr32(reg, v);
    return GPIO_BASIC_OK;
}

int TMS320C6452_DSP_gpio_basic_int_clear(const TMS320C6452_DSP_CoreAPI *core, uint32_t pin)
{
    if ((pin > 31u))
        return GPIO_BASIC_ERR_BAD_PIN;
    if (!_chk_known(GPIO_INT_CLR_OFFSET))
        return GPIO_BASIC_ERR_UNKNOWN_REG; // UNKNOWN

    uintptr_t reg = TMS320C6452_DSP_REG(GPIO_INT_CLR_OFFSET);
    uint32_t m = GPIO_PIN_MASK(pin);
    core->wr32(reg, m); // write-1-to-clear assumed when UNKNOWN
    return GPIO_BASIC_OK;
}
