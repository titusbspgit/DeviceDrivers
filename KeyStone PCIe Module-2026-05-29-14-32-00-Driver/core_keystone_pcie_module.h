/*
 * Core Driver Header for KeyStone PCIe Module
 * Author: ag-drv-CodeGen Agent
 * Date: 2026-05-29
 * Environment: Baremetal | Arch: ARM | Compiler: gcc | Endianness: Big Endian
 * Notes:
 *  - This core provides MMIO helpers with big-endian semantics and minimal barriers.
 *  - Register offsets are UNKNOWN unless defined by the official specification.
 */
#ifndef CORE_KEYSTONE_PCIE_MODULE_H
#define CORE_KEYSTONE_PCIE_MODULE_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Base address of the KeyStone PCIe Module */
#define KPM_BASE_ADDR ((uintptr_t)0xA1008000u)

/* Endianness configuration (Big Endian) */
#define KPM_CFG_BIG_ENDIAN 1

/* Status codes */
typedef enum {
    KPM_STATUS_OK = 0,
    KPM_STATUS_ERROR = -1,
    KPM_STATUS_TIMEOUT = -2,
    KPM_STATUS_INVALID_ARG = -3,
    KPM_STATUS_UNSUPPORTED = -4
} kpm_status_t;

/* Forward declaration */
struct KPM_CoreAPI;

typedef struct KPM_Core {
    uintptr_t base;
} KPM_Core;

/* Core API providing abstracted register access for feature drivers */
typedef struct KPM_CoreAPI {
    /* Read 32-bit register at byte offset from base (big-endian semantics). */
    uint32_t (*read32)(KPM_Core *core, uint32_t byte_offset);
    /* Write 32-bit register at byte offset from base (big-endian semantics). */
    void     (*write32)(KPM_Core *core, uint32_t byte_offset, uint32_t value);
    /* Read-modify-write helper: (old & ~mask) | (value & mask) */
    void     (*rmw32)(KPM_Core *core, uint32_t byte_offset, uint32_t mask, uint32_t value);
    /* Memory barriers */
    void     (*barrier)(void);
    /* Busy-wait delay (microseconds). Implementation is a simple loop). */
    void     (*udelay)(uint32_t usec);
} KPM_CoreAPI;

/* Public core API instance getter (singleton-style API table) */
const KPM_CoreAPI *kpm_core_get_api(void);

/* Initialize a KPM_Core context with provided base address. */
static inline void kpm_core_init(KPM_Core *core, uintptr_t base_addr)
{
    if (core) {
        core->base = base_addr;
    }
}

/* UNKNOWN register offsets placeholder macros (do not use until defined). */
/* Example: #define KPM_REG_LINK_CTRL2_OFFSET  UNKNOWN  */
/* Missing data -> use official spec to define before enabling access. */

#ifdef __cplusplus
}
#endif

#endif /* CORE_KEYSTONE_PCIE_MODULE_H */
