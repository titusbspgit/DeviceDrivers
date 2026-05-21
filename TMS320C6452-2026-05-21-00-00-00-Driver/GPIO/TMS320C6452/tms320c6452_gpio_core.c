#include "tms320c6452_gpio_core.h"

static inline uint32_t mmio_read32(uintptr_t addr)
{
    return *(volatile uint32_t *)addr;
}

static inline void mmio_write32(uintptr_t addr, uint32_t value)
{
    *(volatile uint32_t *)addr = value;
}

static uint32_t core_rd(uint32_t offset)
{
    return mmio_read32(TMS320C6452_GPIO_BASE + (uintptr_t)offset);
}

static void core_wr(uint32_t offset, uint32_t value)
{
    mmio_write32(TMS320C6452_GPIO_BASE + (uintptr_t)offset, value);
}

const tms320c6452_gpio_core_if_t tms320c6452_gpio_core = {
    .base = TMS320C6452_GPIO_BASE,
    .rd   = core_rd,
    .wr   = core_wr,
};
