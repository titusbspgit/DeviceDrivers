/*
 * hal_mmio.h - Minimal MMIO HAL for big-endian ARM bare-metal
 * RCI-ag-DrvGen Agent
 */
#ifndef HAL_MMIO_H
#define HAL_MMIO_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <inttypes.h>
#include <stdio.h>

/* Configured parameters */
#define PCIE_BASE_ADDR   (0xA1008000u)

/* Endianness handling
 * Assumption: Device registers are in CPU native endianness (big-endian).
 * If the controller exposes little-endian registers on a big-endian CPU,
 * define PCIE_REG_LITTLE_ENDIAN to 1 to enable byte-swapping.
 */
#ifndef PCIE_REG_LITTLE_ENDIAN
#define PCIE_REG_LITTLE_ENDIAN (0)
#endif

static inline uint32_t hal_bswap32(uint32_t v)
{
    return (uint32_t)((v << 24)
        | ((v & 0x0000FF00u) << 8)
        | ((v & 0x00FF0000u) >> 8)
        | (v >> 24));
}

static inline uint32_t hal_reg_to_cpu32(uint32_t v)
{
#if PCIE_REG_LITTLE_ENDIAN
    return hal_bswap32(v);
#else
    return v;
#endif
}

static inline uint32_t hal_cpu_to_reg32(uint32_t v)
{
#if PCIE_REG_LITTLE_ENDIAN
    return hal_bswap32(v);
#else
    return v;
#endif
}

#define MMIO_BARRIER() do { __asm__ volatile ("" : : : "memory"); } while (0)

static inline void hal_mmio_write32(uintptr_t addr, uint32_t value)
{
    volatile uint32_t *p = (volatile uint32_t *)addr;
    *p = hal_cpu_to_reg32(value);
    MMIO_BARRIER();
}

static inline uint32_t hal_mmio_read32(uintptr_t addr)
{
    volatile const uint32_t *p = (volatile const uint32_t *)addr;
    uint32_t v = *p;
    MMIO_BARRIER();
    return hal_reg_to_cpu32(v);
}

static inline void hal_mmio_rmw32(uintptr_t addr, uint32_t set_mask, uint32_t clear_mask)
{
    uint32_t v = hal_mmio_read32(addr);
    v &= ~clear_mask;
    v |= set_mask;
    hal_mmio_write32(addr, v);
}

#endif /* HAL_MMIO_H */
