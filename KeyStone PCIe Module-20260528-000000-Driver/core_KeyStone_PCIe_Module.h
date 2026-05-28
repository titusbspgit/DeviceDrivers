#ifndef CORE_KEYSTONE_PCIE_MODULE_H
#define CORE_KEYSTONE_PCIE_MODULE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Base address for KeyStone PCIe Module */
#define KPM_BASE_ADDR            (0xA1008000u)

/* Sentinel for unknown/unspecified register offsets or bitfields */
#define KPM_REG_UNKNOWN          (0xFFFFFFFFu)

/* Common error codes for feature drivers */
#define KPM_OK         (0)
#define KPM_EINVAL     (-1)
#define KPM_ENOTSUP    (-2)
#define KPM_EIO        (-3)
#define KPM_ETIME      (-4)

/* Memory barriers for baremetal ARM */
static inline void kpm_mb(void)  { __asm__ volatile ("dmb sy" ::: "memory"); }
static inline void kpm_rmb(void) { __asm__ volatile ("dmb sy" ::: "memory"); }
static inline void kpm_wmb(void) { __asm__ volatile ("dmb sy" ::: "memory"); }

/* Big-endian MMIO helpers (CPU may be LE/BE; MMIO assumed big-endian as configured) */
static inline uint32_t kpm_bswap32(uint32_t v) { return __builtin_bswap32(v); }

static inline uint32_t kpm_mmio_read32_be(uintptr_t addr)
{
    kpm_mb();
    volatile const uint32_t *p = (volatile const uint32_t *)addr;
    uint32_t v = *p;               /* raw bus order */
    v = kpm_bswap32(v);            /* convert to CPU order */
    kpm_rmb();
    return v;
}

static inline void kpm_mmio_write32_be(uintptr_t addr, uint32_t v)
{
    kpm_wmb();
    volatile uint32_t *p = (volatile uint32_t *)addr;
    *p = kpm_bswap32(v);           /* convert to bus order */
    kpm_mb();
}

/* Core register accessors using offsets from base */
static inline uint32_t kpm_reg_read32(uintptr_t offset)
{
    if (offset == KPM_REG_UNKNOWN) return 0u; /* undefined; caller should treat as ENOTSUP */
    return kpm_mmio_read32_be((uintptr_t)KPM_BASE_ADDR + offset);
}

static inline void kpm_reg_write32(uintptr_t offset, uint32_t val)
{
    if (offset == KPM_REG_UNKNOWN) return; /* undefined; caller should treat as ENOTSUP */
    kpm_mmio_write32_be((uintptr_t)KPM_BASE_ADDR + offset, val);
}

/* Core API for feature drivers */
typedef struct {
    uintptr_t base;
    uint32_t (*read32)(uintptr_t off);
    void     (*write32)(uintptr_t off, uint32_t val);
} kpm_core_api_t;

static inline kpm_core_api_t kpm_core_get_api(void)
{
    kpm_core_api_t api = {
        .base   = KPM_BASE_ADDR,
        .read32 = kpm_reg_read32,
        .write32= kpm_reg_write32,
    };
    return api;
}

#ifdef __cplusplus
}
#endif

#endif /* CORE_KEYSTONE_PCIE_MODULE_H */
