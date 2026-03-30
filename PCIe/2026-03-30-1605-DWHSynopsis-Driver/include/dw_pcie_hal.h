#ifndef DW_PCIE_HAL_H
#define DW_PCIE_HAL_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "dw_pcie_cfg.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Byte-swap helpers */
static inline uint16_t dw_bswap16(uint16_t v) { return (uint16_t)__builtin_bswap16(v); }
static inline uint32_t dw_bswap32(uint32_t v) { return (uint32_t)__builtin_bswap32(v); }

/* Convert between device (DBI) and CPU ordering */
static inline uint16_t dw_from_dev16(uint16_t v)
{
#if (DW_PCIE_CPU_BIG_ENDIAN == 1u) && (DW_PCIE_DEVICE_LITTLE_ENDIAN == 1u)
    return dw_bswap16(v);
#else
    return v;
#endif
}

static inline uint32_t dw_from_dev32(uint32_t v)
{
#if (DW_PCIE_CPU_BIG_ENDIAN == 1u) && (DW_PCIE_DEVICE_LITTLE_ENDIAN == 1u)
    return dw_bswap32(v);
#else
    return v;
#endif
}

static inline uint16_t dw_to_dev16(uint16_t v)
{
#if (DW_PCIE_CPU_BIG_ENDIAN == 1u) && (DW_PCIE_DEVICE_LITTLE_ENDIAN == 1u)
    return dw_bswap16(v);
#else
    return v;
#endif
}

static inline uint32_t dw_to_dev32(uint32_t v)
{
#if (DW_PCIE_CPU_BIG_ENDIAN == 1u) && (DW_PCIE_DEVICE_LITTLE_ENDIAN == 1u)
    return dw_bswap32(v);
#else
    return v;
#endif
}

/* DBI MMIO accessors (deterministic, no UB) */
static inline uint32_t dw_reg_read32(uintptr_t base, uint32_t off)
{
    const volatile uint32_t *addr = (const volatile uint32_t *)(base + (uintptr_t)off);
    uint32_t raw = *addr; /* single 32-bit load */
    DW_DMB();
    return dw_from_dev32(raw);
}

static inline void dw_reg_write32(uintptr_t base, uint32_t off, uint32_t val)
{
    volatile uint32_t *addr = (volatile uint32_t *)(base + (uintptr_t)off);
    *addr = dw_to_dev32(val); /* single 32-bit store */
    DW_DSB();
}

/* 16/8-bit helpers via 32-bit accesses */
static inline uint16_t dw_reg_read16(uintptr_t base, uint32_t off)
{
    uint32_t aligned = off & ~0x3u;
    uint32_t shift = (uint32_t)((off & 0x2u) * 8u);
    uint32_t v32 = dw_reg_read32(base, aligned);
    uint16_t v16 = (uint16_t)((v32 >> shift) & 0xFFFFu);
    return v16;
}

static inline void dw_reg_write16(uintptr_t base, uint32_t off, uint16_t val)
{
    uint32_t aligned = off & ~0x3u;
    uint32_t shift = (uint32_t)((off & 0x2u) * 8u);
    uint32_t cur = dw_reg_read32(base, aligned);
    uint32_t mask = (uint32_t)0xFFFFu << shift;
    uint32_t next = (cur & ~mask) | (((uint32_t)val & 0xFFFFu) << shift);
    dw_reg_write32(base, aligned, next);
}

static inline uint8_t dw_reg_read8(uintptr_t base, uint32_t off)
{
    uint32_t aligned = off & ~0x3u;
    uint32_t shift = (uint32_t)((off & 0x3u) * 8u);
    uint32_t v32 = dw_reg_read32(base, aligned);
    uint8_t v8 = (uint8_t)((v32 >> shift) & 0xFFu);
    return v8;
}

static inline void dw_reg_write8(uintptr_t base, uint32_t off, uint8_t val)
{
    uint32_t aligned = off & ~0x3u;
    uint32_t shift = (uint32_t)((off & 0x3u) * 8u);
    uint32_t cur = dw_reg_read32(base, aligned);
    uint32_t mask = (uint32_t)0xFFu << shift;
    uint32_t next = (cur & ~mask) | (((uint32_t)val & 0xFFu) << shift);
    dw_reg_write32(base, aligned, next);
}

/* W1C helper */
static inline void dw_reg_write1c(uintptr_t base, uint32_t off, uint32_t mask)
{
    dw_reg_write32(base, off, mask);
}

/* Device descriptor */
typedef struct
{
    uintptr_t dbi_base;   /* DBI window base address */
    uint16_t  pcie_cap;   /* Cached PCIe capability base offset */
} dw_pcie_dev_t;

/* Common init: cache PCIe capability offset if present */
int dw_pcie_init(dw_pcie_dev_t *dev, uintptr_t dbi_base);

#ifdef __cplusplus
}
#endif

#endif /* DW_PCIE_HAL_H */
