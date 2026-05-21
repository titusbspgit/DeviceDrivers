#include "tms320c6452_gpio_core.h"

/* Memory barriers for deterministic MMIO ordering */
#ifndef GPIO_MB
#define GPIO_MB() __sync_synchronize()
#endif

static inline volatile uint32_t* gpio_reg_ptr(uint32_t offset)
{
    return (volatile uint32_t*)(uintptr_t)(TMS320C6452_GPIO_BASE + offset);
}

static uint32_t core_read_reg(uint32_t offset)
{
    uint32_t v = *gpio_reg_ptr(offset);
    GPIO_MB();
    return v;
}

static void core_write_reg(uint32_t offset, uint32_t val)
{
    GPIO_MB();
    *gpio_reg_ptr(offset) = val;
    GPIO_MB();
}

static void core_set_bits(uint32_t offset, uint32_t mask)
{
    uint32_t v = core_read_reg(offset);
    core_write_reg(offset, v | mask);
}

static void core_clear_bits(uint32_t offset, uint32_t mask)
{
    uint32_t v = core_read_reg(offset);
    core_write_reg(offset, v & ~mask);
}

static const tms320c6452_gpio_core_api_t s_core_api = {
    .write_reg  = core_write_reg,
    .read_reg   = core_read_reg,
    .set_bits   = core_set_bits,
    .clear_bits = core_clear_bits,
};

const tms320c6452_gpio_core_api_t* tms320c6452_gpio_core_get(void)
{
    return &s_core_api;
}
