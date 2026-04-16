// SPDX-License-Identifier: MIT
// RCI-ag-DrvGen Agent - HAL MMIO Accessors
// Controller: TMS320C6452 DSP
// Endianness: Big-endian (driver is endian-safe)
#ifndef RCI_HAL_REG_ACCESS_H
#define RCI_HAL_REG_ACCESS_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// Compiler barrier
#ifndef RCI_COMPILER_BARRIER
#define RCI_COMPILER_BARRIER() __asm__ __volatile__("" ::: "memory")
#endif

// Endianness conversion helpers (device registers are naturally big-endian fields)
#ifndef RCI_BE16
#define RCI_BE16(x) ((uint16_t)(
    (((uint16_t)(x) & (uint16_t)0x00FFu) << 8) |
    (((uint16_t)(x) & (uint16_t)0xFF00u) >> 8)))
#endif

#ifndef RCI_BE32
#define RCI_BE32(x) ((uint32_t)(
    (((uint32_t)(x) & 0x000000FFu) << 24) |
    (((uint32_t)(x) & 0x0000FF00u) << 8)  |
    (((uint32_t)(x) & 0x00FF0000u) >> 8)  |
    (((uint32_t)(x) & 0xFF000000u) >> 24)))
#endif

#if defined(__BYTE_ORDER__) && defined(__ORDER_BIG_ENDIAN__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#define RCI_CPU_BIG_ENDIAN 1
#else
#define RCI_CPU_BIG_ENDIAN 0
#endif

static inline uint16_t rci_to_be16(uint16_t v)
{
#if RCI_CPU_BIG_ENDIAN
    return v;
#else
    return RCI_BE16(v);
#endif
}

static inline uint16_t rci_from_be16(uint16_t v)
{
#if RCI_CPU_BIG_ENDIAN
    return v;
#else
    return RCI_BE16(v);
#endif
}

static inline uint32_t rci_to_be32(uint32_t v)
{
#if RCI_CPU_BIG_ENDIAN
    return v;
#else
    return RCI_BE32(v);
#endif
}

static inline uint32_t rci_from_be32(uint32_t v)
{
#if RCI_CPU_BIG_ENDIAN
    return v;
#else
    return RCI_BE32(v);
#endif
}

// Optional MMIO mock for unit tests
#ifdef RCI_MMIO_MOCK
#include <string.h>
void rci_mock_init(uintptr_t base, uint8_t *buffer, size_t size_bytes);
#endif

// Raw MMIO accessors (8/16/32). Offsets/addresses must be naturally aligned.
static inline uint8_t rci_reg_read8(uintptr_t addr)
{
#ifdef RCI_MMIO_MOCK
    extern uint8_t *rci_mmio_mock_buf;
    extern uintptr_t rci_mmio_mock_base;
    extern size_t rci_mmio_mock_size;
    if ((addr < rci_mmio_mock_base) || (addr - rci_mmio_mock_base >= rci_mmio_mock_size)) {
        return 0u;
    }
    const size_t off = (size_t)(addr - rci_mmio_mock_base);
    RCI_COMPILER_BARRIER();
    return rci_mmio_mock_buf[off];
#else
    volatile const uint8_t *p = (volatile const uint8_t *)addr;
    RCI_COMPILER_BARRIER();
    return *p;
#endif
}

static inline void rci_reg_write8(uintptr_t addr, uint8_t v)
{
#ifdef RCI_MMIO_MOCK
    extern uint8_t *rci_mmio_mock_buf;
    extern uintptr_t rci_mmio_mock_base;
    extern size_t rci_mmio_mock_size;
    if ((addr < rci_mmio_mock_base) || (addr - rci_mmio_mock_base >= rci_mmio_mock_size)) {
        return;
    }
    const size_t off = (size_t)(addr - rci_mmio_mock_base);
    RCI_COMPILER_BARRIER();
    rci_mmio_mock_buf[off] = v;
    RCI_COMPILER_BARRIER();
#else
    volatile uint8_t *p = (volatile uint8_t *)addr;
    RCI_COMPILER_BARRIER();
    *p = v;
    RCI_COMPILER_BARRIER();
#endif
}

static inline uint16_t rci_reg_read16(uintptr_t addr)
{
#ifdef RCI_MMIO_MOCK
    uint16_t be = ((uint16_t)rci_reg_read8(addr) << 8) | (uint16_t)rci_reg_read8(addr + (uintptr_t)1);
    return rci_from_be16(be);
#else
    volatile const uint16_t *p = (volatile const uint16_t *)addr;
    RCI_COMPILER_BARRIER();
#if RCI_CPU_BIG_ENDIAN
    return *p;
#else
    return rci_from_be16(*p);
#endif
#endif
}

static inline void rci_reg_write16(uintptr_t addr, uint16_t v)
{
#ifdef RCI_MMIO_MOCK
    uint16_t be = rci_to_be16(v);
    rci_reg_write8(addr, (uint8_t)((be >> 8) & 0xFFu));
    rci_reg_write8(addr + (uintptr_t)1, (uint8_t)(be & 0xFFu));
#else
    volatile uint16_t *p = (volatile uint16_t *)addr;
    RCI_COMPILER_BARRIER();
#if RCI_CPU_BIG_ENDIAN
    *p = v;
#else
    *p = rci_to_be16(v);
#endif
    RCI_COMPILER_BARRIER();
#endif
}

static inline uint32_t rci_reg_read32(uintptr_t addr)
{
#ifdef RCI_MMIO_MOCK
    uint32_t be = ((uint32_t)rci_reg_read8(addr) << 24) |
                  ((uint32_t)rci_reg_read8(addr + (uintptr_t)1) << 16) |
                  ((uint32_t)rci_reg_read8(addr + (uintptr_t)2) << 8) |
                  ((uint32_t)rci_reg_read8(addr + (uintptr_t)3));
    return rci_from_be32(be);
#else
    volatile const uint32_t *p = (volatile const uint32_t *)addr;
    RCI_COMPILER_BARRIER();
#if RCI_CPU_BIG_ENDIAN
    return *p;
#else
    return rci_from_be32(*p);
#endif
#endif
}

static inline void rci_reg_write32(uintptr_t addr, uint32_t v)
{
#ifdef RCI_MMIO_MOCK
    uint32_t be = rci_to_be32(v);
    rci_reg_write8(addr, (uint8_t)((be >> 24) & 0xFFu));
    rci_reg_write8(addr + (uintptr_t)1, (uint8_t)((be >> 16) & 0xFFu));
    rci_reg_write8(addr + (uintptr_t)2, (uint8_t)((be >> 8) & 0xFFu));
    rci_reg_write8(addr + (uintptr_t)3, (uint8_t)(be & 0xFFu));
#else
    volatile uint32_t *p = (volatile uint32_t *)addr;
    RCI_COMPILER_BARRIER();
#if RCI_CPU_BIG_ENDIAN
    *p = v;
#else
    *p = rci_to_be32(v);
#endif
    RCI_COMPILER_BARRIER();
#endif
}

#ifdef __cplusplus
}
#endif

#endif // RCI_HAL_REG_ACCESS_H
