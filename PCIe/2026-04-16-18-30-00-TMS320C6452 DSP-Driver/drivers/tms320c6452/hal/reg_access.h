/* SPDX-License-Identifier: MIT */
/* File: drivers/tms320c6452/hal/reg_access.h */
/* RCI-ag-DrvGen Agent - Endian-safe MMIO accessors with optional mock */
#ifndef RCI_REG_ACCESS_H
#define RCI_REG_ACCESS_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Configuration: target is big-endian per requirements */
#ifndef RCI_CONFIG_BIG_ENDIAN
#define RCI_CONFIG_BIG_ENDIAN 1
#endif

/* Compiler barrier to prevent reordering across MMIO */
#ifndef RCI_COMPILER_BARRIER
#define RCI_COMPILER_BARRIER() __asm__ __volatile__("" ::: "memory")
#endif

/* Endianness helpers (compile-time selection) */
static inline uint16_t rci_bswap16(uint16_t v) { return (uint16_t)((v << 8) | (v >> 8)); }
static inline uint32_t rci_bswap32(uint32_t v) {
    return ((v & 0x000000FFu) << 24) |
           ((v & 0x0000FF00u) << 8)  |
           ((v & 0x00FF0000u) >> 8)  |
           ((v & 0xFF000000u) >> 24);
}

#if RCI_CONFIG_BIG_ENDIAN
#define rci_cpu_to_be16(x)   (uint16_t)(x)
#define rci_be16_to_cpu(x)   (uint16_t)(x)
#define rci_cpu_to_be32(x)   (uint32_t)(x)
#define rci_be32_to_cpu(x)   (uint32_t)(x)
#else
#define rci_cpu_to_be16(x)   rci_bswap16((uint16_t)(x))
#define rci_be16_to_cpu(x)   rci_bswap16((uint16_t)(x))
#define rci_cpu_to_be32(x)   rci_bswap32((uint32_t)(x))
#define rci_be32_to_cpu(x)   rci_bswap32((uint32_t)(x))
#endif

/* Public MMIO API */
uint8_t  rci_reg_read8 (uintptr_t addr);
uint16_t rci_reg_read16(uintptr_t addr);
uint32_t rci_reg_read32(uintptr_t addr);

void rci_reg_write8 (uintptr_t addr, uint8_t  val);
void rci_reg_write16(uintptr_t addr, uint16_t val);
void rci_reg_write32(uintptr_t addr, uint32_t val);

/* Optional MMIO mock for unit tests */
#ifdef RCI_MMIO_MOCK
void rci_mock_init(uintptr_t base_addr, uint8_t *space, size_t space_size);
#endif

#ifdef __cplusplus
}
#endif

#endif /* RCI_REG_ACCESS_H */
