/*
 * Synopsys DesignWare PCIe (DM) v6.00a - Link Training and Equalization Controls (Vendor DSP)
 * Source
 *
 * Note: Bitfields vary with IP configuration. Provide generic masked RMW APIs.
 */
#include "dw_pcie_dm_link_eq.h"

#ifndef DW_PCIE_DM_BIG_ENDIAN
#define CPU_TO_LE32(x) ((uint32_t)(x))
#define LE32_TO_CPU(x) ((uint32_t)(x))
#else
#define CPU_TO_LE32(x) (__builtin_bswap32((uint32_t)(x)))
#define LE32_TO_CPU(x) (__builtin_bswap32((uint32_t)(x)))
#endif

static inline uint32_t mmio_read32(const volatile void *addr)
{
    const volatile uint32_t *p = (const volatile uint32_t *)addr;
    return LE32_TO_CPU(*p);
}

static inline void mmio_write32(volatile void *addr, uint32_t value)
{
    volatile uint32_t *p = (volatile uint32_t *)addr;
    *p = CPU_TO_LE32(value);
    (void)*p;
}

int32_t dw_pcie_dm_linkeq_init(dw_pcie_dm_linkeq_ctx_t *ctx, uintptr_t dsp_base)
{
    if ((ctx == (void *)0) || (dsp_base == (uintptr_t)0)) { return DW_PCIE_DM_EINVAL; }
    ctx->dsp_base = (volatile uint8_t *)dsp_base;
    return DW_PCIE_DM_OK;
}

int32_t dw_pcie_dm_linkeq_rmw(const dw_pcie_dm_linkeq_ctx_t *ctx, uint32_t reg_off, uint32_t mask, uint32_t value)
{
    if ((ctx == (void *)0) || (mask == 0u) || ((value & ~mask) != 0u)) { return DW_PCIE_DM_EINVAL; }
    const volatile void *addr = (const void *)(ctx->dsp_base + reg_off);
    const uint32_t cur = mmio_read32(addr);
    const uint32_t newv = (cur & ~mask) | (value & mask);
    mmio_write32((void *)addr, newv);
    const uint32_t rb = mmio_read32(addr);
    return (rb == newv) ? DW_PCIE_DM_OK : DW_PCIE_DM_EBUSY;
}

int32_t dw_pcie_dm_linkeq_write_local_fs_lf(const dw_pcie_dm_linkeq_ctx_t *ctx, uint8_t fs, uint8_t lf)
{
    if (ctx == (void *)0) { return DW_PCIE_DM_EINVAL; }
    /* Common mapping used by Synopsys: FS in [7:0], LF in [15:8] of GEN3_EQ_LOCAL_FS_LF */
    const uint32_t val = ((uint32_t)lf << 8) | ((uint32_t)fs);
    mmio_write32((void *)(ctx->dsp_base + DWC_GEN3_EQ_LOCAL_FS_LF), val);
    const uint32_t rb = mmio_read32((const void *)(ctx->dsp_base + DWC_GEN3_EQ_LOCAL_FS_LF));
    return (rb == val) ? DW_PCIE_DM_OK : DW_PCIE_DM_EBUSY;
}

int32_t dw_pcie_dm_linkeq_port_force(const dw_pcie_dm_linkeq_ctx_t *ctx, uint32_t mask, uint32_t value)
{
    return dw_pcie_dm_linkeq_rmw(ctx, DWC_PORT_FORCE_OFF, mask, value);
}
