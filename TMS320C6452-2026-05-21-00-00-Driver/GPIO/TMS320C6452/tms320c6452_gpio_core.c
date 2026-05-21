#include "tms320c6452_gpio_core.h"

static uint32_t core_rd(uint32_t off)
{
    volatile uint32_t *addr = (volatile uint32_t *)(TMS320C6452_GPIO_BASE + (uintptr_t)off);
    return *addr;
}

static void core_wr(uint32_t off, uint32_t val)
{
    volatile uint32_t *addr = (volatile uint32_t *)(TMS320C6452_GPIO_BASE + (uintptr_t)off);
    *addr = val;
}

const tms320c6452_gpio_core_api_t tms320c6452_gpio_core = {
    .rd   = core_rd,
    .wr   = core_wr,
    .base = TMS320C6452_GPIO_BASE,
};
