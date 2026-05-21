#include "tms320c6452_gpio_gpio_banked_architecture_(32_signals,_2_banks).h"

void tms320c6452_gpio_banked_init(void)
{
    /* Deterministic initialization: disable bank interrupts; clear edge enables. */
    uint32_t tmp;

    /* Disable per-bank interrupt gates */
    tmp = HWREG32(GPIO_BINTEN);
    tmp &= ~(GPIO_BINTEN_EN_BANK0 | GPIO_BINTEN_EN_BANK1);
    HWREG32(GPIO_BINTEN) = tmp;

    /* Disable rising/falling edge enables for all pins */
    HWREG32(GPIO_CLR_RIS_TRIG01) = 0xFFFFFFFFu;
    HWREG32(GPIO_CLR_FAL_TRIG01) = 0xFFFFFFFFu;
}

void tms320c6452_gpio_bank_enable_interrupt(uint32_t bank, bool enable)
{
    uint32_t mask = (bank == 0u) ? GPIO_BINTEN_EN_BANK0 : GPIO_BINTEN_EN_BANK1;
    uint32_t val = HWREG32(GPIO_BINTEN);
    if (enable) {
        val |= mask;
    } else {
        val &= ~mask;
    }
    HWREG32(GPIO_BINTEN) = val;
}

void tms320c6452_gpio_write_mask(uint32_t set_mask, uint32_t clr_mask)
{
    /* Atomic set then clear as requested */
    if (set_mask) {
        HWREG32(GPIO_SET_DATA01) = set_mask;
    }
    if (clr_mask) {
        HWREG32(GPIO_CLR_DATA01) = clr_mask;
    }
}

void tms320c6452_gpio_enable_rising(uint32_t mask)
{
    HWREG32(GPIO_SET_RIS_TRIG01) = mask;
}

void tms320c6452_gpio_disable_rising(uint32_t mask)
{
    HWREG32(GPIO_CLR_RIS_TRIG01) = mask;
}

void tms320c6452_gpio_enable_falling(uint32_t mask)
{
    HWREG32(GPIO_SET_FAL_TRIG01) = mask;
}

void tms320c6452_gpio_disable_falling(uint32_t mask)
{
    HWREG32(GPIO_CLR_FAL_TRIG01) = mask;
}

uint32_t tms320c6452_gpio_read_inputs(void)
{
    return HWREG32(GPIO_IN_DATA01);
}

uint32_t tms320c6452_gpio_read_outputs(void)
{
    return HWREG32(GPIO_OUT_DATA01);
}

uint32_t tms320c6452_gpio_get_int_status(void)
{
    /* Read-only latched status per pin; clearing is device-specific */
    return HWREG32(GPIO_INTSTAT01);
}
