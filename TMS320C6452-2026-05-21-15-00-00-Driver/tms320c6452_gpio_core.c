#include "tms320c6452_gpio_core.h"

static uint32_t core_read32_adapter(uintptr_t base, uint32_t offset)
{
    return tms320c6452_gpio_core_read(base, offset);
}

static void core_write32_adapter(uintptr_t base, uint32_t offset, uint32_t val)
{
    tms320c6452_gpio_core_write(base, offset, val);
}

static const tms320c6452_gpio_core_t g_core = {
    .base   = TMS320C6452_GPIO_BASE,
    .read32 = core_read32_adapter,
    .write32= core_write32_adapter,
};

const tms320c6452_gpio_core_t* tms320c6452_gpio_core_get(void)
{
    return &g_core;
}
