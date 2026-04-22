/*
 * TMS320C6452 DSP GPIO Core Driver (u-boot)
 *
 * Provides deterministic MMIO helpers for big-endian systems and an ops table
 * to be used exclusively by per-feature GPIO drivers.
 */
#include "TMS320C6452 DSP_core.h"

#ifndef __always_inline
#define __always_inline __attribute__((always_inline)) inline
#endif

/* Internal base captured via core_init */
static volatile uintptr_t g_gpio_base = (uintptr_t)0u;

static __always_inline void mmio_barrier(void) { __asm__ __volatile__("" ::: "memory"); }

static uint32_t be_mmio_read32(uintptr_t addr)
{
    volatile uint32_t *p = (volatile uint32_t *)addr;
    mmio_barrier();
    uint32_t v = *p; /* CPU and bus are big-endian */
    mmio_barrier();
    return v;
}

static void be_mmio_write32(uintptr_t addr, uint32_t val)
{
    volatile uint32_t *p = (volatile uint32_t *)addr;
    mmio_barrier();
    *p = val; /* CPU and bus are big-endian */
    mmio_barrier();
}

static void core_init_impl(uintptr_t base)
{
    g_gpio_base = base;
}

static uint32_t read_reg_impl(uintptr_t offset)
{
    return be_mmio_read32(g_gpio_base + offset);
}

static void write_reg_impl(uintptr_t offset, uint32_t value)
{
    be_mmio_write32(g_gpio_base + offset, value);
}

static void set_bits_impl(uintptr_t offset, uint32_t mask)
{
    uint32_t v = be_mmio_read32(g_gpio_base + offset);
    v |= mask;
    be_mmio_write32(g_gpio_base + offset, v);
}

static void clear_bits_impl(uintptr_t offset, uint32_t mask)
{
    uint32_t v = be_mmio_read32(g_gpio_base + offset);
    v &= (uint32_t)(~mask);
    be_mmio_write32(g_gpio_base + offset, v);
}

static void rmw_impl(uintptr_t offset, uint32_t mask, uint32_t value)
{
    uint32_t v = be_mmio_read32(g_gpio_base + offset);
    v = (uint32_t)((v & (~mask)) | (value & mask));
    be_mmio_write32(g_gpio_base + offset, v);
}

static void delay_us_impl(uint32_t usec)
{
    /* Stub: platform timer hookup required in u-boot integration */
    (void)usec;
}

const gpio_core_ops_t gpio_core_ops = {
    .core_init         = core_init_impl,
    .read_reg          = read_reg_impl,
    .write_reg         = write_reg_impl,
    .set_bits          = set_bits_impl,
    .clear_bits        = clear_bits_impl,
    .read_modify_write = rmw_impl,
    .delay_us          = delay_us_impl,
};
