/* Core driver source for KeyStone PCIe Module (Big-Endian, ARM, Baremetal, GCC) */
#include "core_KeyStone PCIe Module.h"

/*
 * Note on endianness:
 * - The module is big-endian. If the toolchain targets little-endian, values are byte-swapped on access.
 * - If the toolchain targets big-endian, accesses are direct (no swap performed).
 */

static const uintptr_t ks_pcie_base = KS_PCIE_BASE_ADDR;

static inline volatile uint8_t*  ks_reg8 (uintptr_t off) { return (volatile uint8_t *)(ks_pcie_base + off); }
static inline volatile uint16_t* ks_reg16(uintptr_t off) { return (volatile uint16_t*)(ks_pcie_base + off); }
static inline volatile uint32_t* ks_reg32(uintptr_t off) { return (volatile uint32_t*)(ks_pcie_base + off); }

void ks_pcie_core_init(void)
{
    /* For baremetal, ensure any prior configuration is observed before MMIO. */
    KS_BARRIER_DSB();
    KS_BARRIER_ISB();
}

uint8_t ks_pcie_read8(uintptr_t offset)
{
    uint8_t v = *ks_reg8(offset);
    KS_BARRIER_DSB();
    return v; /* 8-bit accesses are endian-agnostic */
}

uint16_t ks_pcie_read16(uintptr_t offset)
{
    uint16_t be = *ks_reg16(offset);
    KS_BARRIER_DSB();
    return ks_be16_to_cpu(be);
}

uint32_t ks_pcie_read32(uintptr_t offset)
{
    uint32_t be = *ks_reg32(offset);
    KS_BARRIER_DSB();
    return ks_be32_to_cpu(be);
}

void ks_pcie_write8(uintptr_t offset, uint8_t value)
{
    *ks_reg8(offset) = value;
    KS_BARRIER_DSB();
}

void ks_pcie_write16(uintptr_t offset, uint16_t value)
{
    *ks_reg16(offset) = ks_cpu_to_be16(value);
    KS_BARRIER_DSB();
}

void ks_pcie_write32(uintptr_t offset, uint32_t value)
{
    *ks_reg32(offset) = ks_cpu_to_be32(value);
    KS_BARRIER_DSB();
}

static const ks_pcie_core_api_t g_core_api = {
    .init    = ks_pcie_core_init,
    .read8   = ks_pcie_read8,
    .read16  = ks_pcie_read16,
    .read32  = ks_pcie_read32,
    .write8  = ks_pcie_write8,
    .write16 = ks_pcie_write16,
    .write32 = ks_pcie_write32,
    .fence   = ks_pcie_fence,
    .base    = KS_PCIE_BASE_ADDR,
};

const ks_pcie_core_api_t* ks_pcie_core_get_api(void)
{
    return &g_core_api;
}
