/* Core driver for KeyStone PCIe Module - Baremetal, ARM, GCC, Big-endian CPU */
#ifndef CORE_KEYSTONE_PCIE_MODULE_H
#define CORE_KEYSTONE_PCIE_MODULE_H

#include <stdint.h>
#include <stddef.h>

/* Base address */
#define KS_PCIE_BASE_ADDR   ((uintptr_t)0xA1008000u)

/* CPU endianness: Big-endian */
#define KS_PCIE_CPU_BIG_ENDIAN   1

/* Register endianness hook:
 * If the hardware registers are little-endian relative to a big-endian CPU,
 * define KS_PCIE_REGS_LITTLE_ENDIAN to enable byte-swapping on access.
 */
/* #define KS_PCIE_REGS_LITTLE_ENDIAN 1 */

/* Byte-swap helpers (compile-time no-op when not needed) */
static inline uint32_t ks_pcie_bswap32(uint32_t v)
{
#if defined(__GNUC__)
    return __builtin_bswap32(v);
#else
    return ((v & 0x000000FFu) << 24) |
           ((v & 0x0000FF00u) << 8)  |
           ((v & 0x00FF0000u) >> 8)  |
           ((v & 0xFF000000u) >> 24);
#endif
}

#if defined(KS_PCIE_CPU_BIG_ENDIAN) && defined(KS_PCIE_REGS_LITTLE_ENDIAN)
  #define KS_TO_REG32(v)   ks_pcie_bswap32((uint32_t)(v))
  #define KS_FROM_REG32(v) ks_pcie_bswap32((uint32_t)(v))
#else
  #define KS_TO_REG32(v)   ((uint32_t)(v))
  #define KS_FROM_REG32(v) ((uint32_t)(v))
#endif

/* Memory barrier */
static inline void ks_pcie_dmb(void)
{
#if defined(__arm__) || defined(__aarch64__)
    __asm__ volatile ("dmb sy" ::: "memory");
#else
    __sync_synchronize();
#endif
}

/* MMIO register accessor */
#define KS_PCIE_REG32(off) (*(volatile uint32_t *)((KS_PCIE_BASE_ADDR) + (uintptr_t)(off)))

/* Core API used by feature drivers */
typedef struct ks_pcie_core_api {
    /* 32-bit read/write (offset from module base) */
    uint32_t (*read32)(uint32_t offset);
    void     (*write32)(uint32_t offset, uint32_t value);
    /* Bit operations */
    void     (*set_bits)(uint32_t offset, uint32_t mask);
    void     (*clear_bits)(uint32_t offset, uint32_t mask);
    /* Poll until (reg & mask) == value, timeout in microseconds; returns 0 on success, -1 on timeout */
    int      (*poll32)(uint32_t offset, uint32_t mask, uint32_t value, uint32_t timeout_us);
} ks_pcie_core_api_t;

#ifdef __cplusplus
extern "C" {
#endif

/* Initialize core (idempotent) */
void ks_pcie_core_init(void);

/* Obtain pointer to core API table */
const ks_pcie_core_api_t * ks_pcie_core(void);

/* Direct helpers for convenience */
uint32_t ks_pcie_read32(uint32_t offset);
void     ks_pcie_write32(uint32_t offset, uint32_t value);
void     ks_pcie_set_bits(uint32_t offset, uint32_t mask);
void     ks_pcie_clear_bits(uint32_t offset, uint32_t mask);
int      ks_pcie_poll32(uint32_t offset, uint32_t mask, uint32_t value, uint32_t timeout_us);

#ifdef __cplusplus
}
#endif

#endif /* CORE_KEYSTONE_PCIE_MODULE_H */
