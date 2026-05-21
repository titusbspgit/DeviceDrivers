#include "tms320c6452_gpio_core.h"

void tms320c6452_gpio_core_init(tms320c6452_gpio_core_t *core, uintptr_t base)
{
    if (core) { core->base = base; }
}

void tms320c6452_gpio_writel(tms320c6452_gpio_core_t *core, uint32_t ofs, uint32_t val)
{
    *(volatile uint32_t *)(core->base + (uintptr_t)ofs) = val;
}

uint32_t tms320c6452_gpio_readl(tms320c6452_gpio_core_t *core, uint32_t ofs)
{
    return *(volatile uint32_t *)(core->base + (uintptr_t)ofs);
}
