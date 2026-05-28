/* Core driver header for KeyStone PCIe Module (Big-Endian, ARM, Baremetal, GCC) */
#ifndef CORE_KEYSTONE_PCIE_MODULE_H
#define CORE_KEYSTONE_PCIE_MODULE_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Base address (memory-mapped I/O) */
#define KS_PCIE_BASE_ADDR   ((uintptr_t)0xA1008000u)

/* Memory barriers for strongly ordered MMIO semantics */
#if defined(__GNUC__)
#define KS_BARRIER_DSB() __asm__ __volatile__("dsb sy" ::: "memory")
#define KS_BARRIER_ISB() __asm__ __volatile__("isb sy" ::: "memory")
#else
#define KS_BARRIER_DSB() do { } while (0)
#define KS_BARRIER_ISB() do { } while (0)
#endif

/* Endian helpers: ensure big-endian device ordering regardless of host endianness */
static inline uint16_t ks_be16_to_cpu(uint16_t x) {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    return __builtin_bswap16(x);
#else
    return x;
#endif
}

static inline uint32_t ks_be32_to_cpu(uint32_t x) {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    return __builtin_bswap32(x);
#else
    return x;
#endif
}

static inline uint16_t ks_cpu_to_be16(uint16_t x) {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    return __builtin_bswap16(x);
#else
    return x;
#endif
}

static inline uint32_t ks_cpu_to_be32(uint32_t x) {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    return __builtin_bswap32(x);
#else
    return x;
#endif
}

/* Raw MMIO accessors (offset is from KS_PCIE_BASE_ADDR) */
uint8_t  ks_pcie_read8 (uintptr_t offset);
uint16_t ks_pcie_read16(uintptr_t offset);
uint32_t ks_pcie_read32(uintptr_t offset);

void ks_pcie_write8 (uintptr_t offset, uint8_t  value);
void ks_pcie_write16(uintptr_t offset, uint16_t value);
void ks_pcie_write32(uintptr_t offset, uint32_t value);

/* Optional fence to guarantee completion ordering of posted writes */
static inline void ks_pcie_fence(void) { KS_BARRIER_DSB(); KS_BARRIER_ISB(); }

/* Core API exported for reuse by feature-specific drivers */
typedef struct ks_pcie_core_api_s {
    void     (*init)(void);                 /* Optional low-level init/fence */
    uint8_t  (*read8)(uintptr_t off);
    uint16_t (*read16)(uintptr_t off);
    uint32_t (*read32)(uintptr_t off);
    void     (*write8)(uintptr_t off, uint8_t  val);
    void     (*write16)(uintptr_t off, uint16_t val);
    void     (*write32)(uintptr_t off, uint32_t val);
    void     (*fence)(void);
    uintptr_t base;                         /* KS_PCIE_BASE_ADDR (for reference) */
} ks_pcie_core_api_t;

void ks_pcie_core_init(void);
const ks_pcie_core_api_t* ks_pcie_core_get_api(void);

#ifdef __cplusplus
}
#endif

#endif /* CORE_KEYSTONE_PCIE_MODULE_H */
