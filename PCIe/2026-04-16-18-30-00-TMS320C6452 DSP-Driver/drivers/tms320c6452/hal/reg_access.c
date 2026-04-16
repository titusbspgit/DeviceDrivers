/* SPDX-License-Identifier: MIT */
/* File: drivers/tms320c6452/hal/reg_access.c */
/* RCI-ag-DrvGen Agent - Endian-safe MMIO accessors with optional mock */
#include "reg_access.h"

#ifndef RCI_MMIO_MOCK

uint8_t rci_reg_read8(uintptr_t addr)
{
    volatile const uint8_t *p = (volatile const uint8_t *)addr;
    uint8_t v = *p;
    RCI_COMPILER_BARRIER();
    return v;
}

uint16_t rci_reg_read16(uintptr_t addr)
{
    volatile const uint16_t *p = (volatile const uint16_t *)addr;
    uint16_t be = *p; /* hardware presents BE; config ensures CPU BE, so no swap */
    RCI_COMPILER_BARRIER();
    return rci_be16_to_cpu(be);
}

uint32_t rci_reg_read32(uintptr_t addr)
{
    volatile const uint32_t *p = (volatile const uint32_t *)addr;
    uint32_t be = *p;
    RCI_COMPILER_BARRIER();
    return rci_be32_to_cpu(be);
}

void rci_reg_write8(uintptr_t addr, uint8_t val)
{
    volatile uint8_t *p = (volatile uint8_t *)addr;
    *p = val;
    RCI_COMPILER_BARRIER();
}

void rci_reg_write16(uintptr_t addr, uint16_t val)
{
    volatile uint16_t *p = (volatile uint16_t *)addr;
    *p = rci_cpu_to_be16(val);
    RCI_COMPILER_BARRIER();
}

void rci_reg_write32(uintptr_t addr, uint32_t val)
{
    volatile uint32_t *p = (volatile uint32_t *)addr;
    *p = rci_cpu_to_be32(val);
    RCI_COMPILER_BARRIER();
}

#else /* RCI_MMIO_MOCK */

/* Simple single-region mock mapping */
static struct {
    uintptr_t base;
    uint8_t *buf;
    size_t size;
    bool inited;
} g_mmio_mock = { 0u, (uint8_t *)0, 0u, false };

void rci_mock_init(uintptr_t base_addr, uint8_t *space, size_t space_size)
{
    g_mmio_mock.base = base_addr;
    g_mmio_mock.buf = space;
    g_mmio_mock.size = space_size;
    g_mmio_mock.inited = true;
}

static inline bool mock_bounds(uintptr_t addr, size_t width)
{
    if (!g_mmio_mock.inited) { return false; }
    if (addr < g_mmio_mock.base) { return false; }
    size_t off = (size_t)(addr - g_mmio_mock.base);
    return (off + width) <= g_mmio_mock.size;
}

uint8_t rci_reg_read8(uintptr_t addr)
{
    if (!mock_bounds(addr, 1u)) { return 0u; }
    uint8_t v = g_mmio_mock.buf[addr - g_mmio_mock.base];
    RCI_COMPILER_BARRIER();
    return v;
}

uint16_t rci_reg_read16(uintptr_t addr)
{
    if (!mock_bounds(addr, 2u)) { return 0u; }
    size_t off = (size_t)(addr - g_mmio_mock.base);
    /* Stored big-endian in buffer */
    uint16_t be = (uint16_t)(((uint16_t)g_mmio_mock.buf[off] << 8) | (uint16_t)g_mmio_mock.buf[off + 1u]);
    RCI_COMPILER_BARRIER();
    return rci_be16_to_cpu(be);
}

uint32_t rci_reg_read32(uintptr_t addr)
{
    if (!mock_bounds(addr, 4u)) { return 0u; }
    size_t off = (size_t)(addr - g_mmio_mock.base);
    uint32_t be = ((uint32_t)g_mmio_mock.buf[off] << 24) |
                  ((uint32_t)g_mmio_mock.buf[off + 1u] << 16) |
                  ((uint32_t)g_mmio_mock.buf[off + 2u] << 8) |
                  ((uint32_t)g_mmio_mock.buf[off + 3u]);
    RCI_COMPILER_BARRIER();
    return rci_be32_to_cpu(be);
}

void rci_reg_write8(uintptr_t addr, uint8_t val)
{
    if (!mock_bounds(addr, 1u)) { return; }
    g_mmio_mock.buf[addr - g_mmio_mock.base] = val;
    RCI_COMPILER_BARRIER();
}

void rci_reg_write16(uintptr_t addr, uint16_t val)
{
    if (!mock_bounds(addr, 2u)) { return; }
    size_t off = (size_t)(addr - g_mmio_mock.base);
    uint16_t be = rci_cpu_to_be16(val);
    g_mmio_mock.buf[off] = (uint8_t)(be >> 8);
    g_mmio_mock.buf[off + 1u] = (uint8_t)(be & 0xFFu);
    RCI_COMPILER_BARRIER();
}

void rci_reg_write32(uintptr_t addr, uint32_t val)
{
    if (!mock_bounds(addr, 4u)) { return; }
    size_t off = (size_t)(addr - g_mmio_mock.base);
    uint32_t be = rci_cpu_to_be32(val);
    g_mmio_mock.buf[off] = (uint8_t)((be >> 24) & 0xFFu);
    g_mmio_mock.buf[off + 1u] = (uint8_t)((be >> 16) & 0xFFu);
    g_mmio_mock.buf[off + 2u] = (uint8_t)((be >> 8) & 0xFFu);
    g_mmio_mock.buf[off + 3u] = (uint8_t)(be & 0xFFu);
    RCI_COMPILER_BARRIER();
}

#endif /* RCI_MMIO_MOCK */
