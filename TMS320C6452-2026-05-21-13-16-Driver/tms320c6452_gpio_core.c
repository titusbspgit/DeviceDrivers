#include "tms320c6452_gpio_core.h"

void tms320c6452_gpio_write32(uint32_t off, uint32_t val)
{
    TMS320C6452_GPIO_REG32(off) = val;
}

uint32_t tms320c6452_gpio_read32(uint32_t off)
{
    return TMS320C6452_GPIO_REG32(off);
}

void tms320c6452_gpio_set_bits(uint32_t off, uint32_t mask)
{
    uint32_t v = TMS320C6452_GPIO_REG32(off);
    v |= mask;
    TMS320C6452_GPIO_REG32(off) = v;
}

void tms320c6452_gpio_clr_bits(uint32_t off, uint32_t mask)
{
    uint32_t v = TMS320C6452_GPIO_REG32(off);
    v &= ~mask;
    TMS320C6452_GPIO_REG32(off) = v;
}

const tms320c6452_gpio_core_if_t tms320c6452_gpio_core = {
    .write32  = tms320c6452_gpio_write32,
    .read32   = tms320c6452_gpio_read32,
    .set_bits = tms320c6452_gpio_set_bits,
    .clr_bits = tms320c6452_gpio_clr_bits,
};
