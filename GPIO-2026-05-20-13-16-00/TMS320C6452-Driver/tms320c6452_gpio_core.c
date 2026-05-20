#include "tms320c6452_gpio_core.h"

static uint32_t core_read32(uintptr_t addr)
{
    volatile uint32_t *p = (volatile uint32_t *)addr;
    uint32_t v = *p;
    __asm__ __volatile__("" ::: "memory");
    return v;
}

static void core_write32(uintptr_t addr, uint32_t val)
{
    __asm__ __volatile__("" ::: "memory");
    volatile uint32_t *p = (volatile uint32_t *)addr;
    *p = val;
    __asm__ __volatile__("" ::: "memory");
}

static void core_barrier(void)
{
    __asm__ __volatile__("" ::: "memory");
}

static const tms320c6452_gpio_core_ops_t s_ops = {
    .read32  = core_read32,
    .write32 = core_write32,
    .barrier = core_barrier,
};

const tms320c6452_gpio_core_ops_t *tms320c6452_gpio_core_get_ops(void)
{
    return &s_ops;
}
