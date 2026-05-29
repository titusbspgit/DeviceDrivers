/*
 * Core Driver Source for KeyStone PCIe Module
 * Author: ag-drv-CodeGen Agent
 * Date: 2026-05-29
 * Environment: Baremetal | Arch: ARM | Compiler: gcc | Endianness: Big Endian
 */
#include "core_keystone_pcie_module.h"

/* Minimal ARM barriers */
static inline void kpm_dmb(void) { __asm__ volatile ("dmb sy" ::: "memory"); }
static inline void kpm_dsb(void) { __asm__ volatile ("dsb sy" ::: "memory"); }
static inline void kpm_isb(void) { __asm__ volatile ("isb" ::: "memory"); }

/* BE32 conversion helpers: CPU is assumed big-endian per configuration. */
static inline uint32_t kpm_cpu_to_be32(uint32_t v) { return v; }
static inline uint32_t kpm_be32_to_cpu(uint32_t v) { return v; }

static uint32_t kpm_read32_be(KPM_Core *core, uint32_t byte_offset)
{
    volatile uint32_t *addr = (volatile uint32_t *)(core->base + (uintptr_t)byte_offset);
    kpm_dmb();
    uint32_t v = *addr;              /* Big-endian CPU: direct load */
    v = kpm_be32_to_cpu(v);
    kpm_isb();
    return v;
}

static void kpm_write32_be(KPM_Core *core, uint32_t byte_offset, uint32_t value)
{
    volatile uint32_t *addr = (volatile uint32_t *)(core->base + (uintptr_t)byte_offset);
    kpm_dmb();
    *addr = kpm_cpu_to_be32(value);  /* Big-endian CPU: direct store */
    kpm_dsb();
}

static void kpm_rmw32_be(KPM_Core *core, uint32_t byte_offset, uint32_t mask, uint32_t value)
{
    uint32_t cur = kpm_read32_be(core, byte_offset);
    cur = (cur & ~mask) | (value & mask);
    kpm_write32_be(core, byte_offset, cur);
}

static void kpm_barrier_impl(void)
{
    kpm_dmb();
    kpm_dsb();
    kpm_isb();
}

static void kpm_udelay_impl(uint32_t usec)
{
    /* Crude delay loop; replace with timer-based delay as needed. */
    volatile uint32_t cnt = usec * 100u;
    while (cnt--) {
        __asm__ volatile ("nop");
    }
}

static const KPM_CoreAPI g_api = {
    .read32  = kpm_read32_be,
    .write32 = kpm_write32_be,
    .rmw32   = kpm_rmw32_be,
    .barrier = kpm_barrier_impl,
    .udelay  = kpm_udelay_impl,
};

const KPM_CoreAPI *kpm_core_get_api(void)
{
    return &g_api;
}
