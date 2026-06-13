/*
 * Synopsys DesignWare PCIe (DM) v6.00a - Secondary PCIe Extended Capability (SPCIE)
 * Source
 */
#include "dw_pcie_dm_spcie.h"
#include <stdio.h>

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

#define PCIE_EXT_CAP_START  (0x100u)

static uint32_t cfg_read32(const dw_pcie_dm_spcie_ctx_t *ctx, uint32_t off)
{
    return mmio_read32((const void *)(ctx->dbi_base + off));
}

static void cfg_write32(const dw_pcie_dm_spcie_ctx_t *ctx, uint32_t off, uint32_t val)
{
    mmio_write32((void *)(ctx->dbi_base + off), val);
}

static uint32_t find_ext_cap(const dw_pcie_dm_spcie_ctx_t *ctx, uint16_t cap_id)
{
    uint32_t off = PCIE_EXT_CAP_START;
    for (uint32_t guard = 0u; (off != 0u) && (off <= 0xFFC0u) && (guard < 64u); ++guard) {
        const uint32_t hdr = cfg_read32(ctx, off + 0x0u);
        const uint16_t id = (uint16_t)(hdr & 0xFFFFu);
        const uint16_t next = (uint16_t)((hdr >> 20) & 0xFFFu);
        if (id == cap_id) { return off; }
        off = (uint32_t)next;
    }
    return 0u;
}

int32_t dw_pcie_dm_spcie_init(dw_pcie_dm_spcie_ctx_t *ctx, uintptr_t dbi_base)
{
    if ((ctx == NULL) || (dbi_base == (uintptr_t)0)) { return DW_PCIE_DM_EINVAL; }
    ctx->dbi_base = (volatile uint8_t *)dbi_base;
    return DW_PCIE_DM_OK;
}

int32_t dw_pcie_dm_spcie_is_present(const dw_pcie_dm_spcie_ctx_t *ctx, bool *present)
{
    if ((ctx == NULL) || (present == NULL)) { return DW_PCIE_DM_EINVAL; }
    const uint32_t off = find_ext_cap(ctx, PCIE_EXT_CAP_ID_SPCIE);
    *present = (off != 0u);
    return DW_PCIE_DM_OK;
}

int32_t dw_pcie_dm_spcie_request_equalization(const dw_pcie_dm_spcie_ctx_t *ctx)
{
    if ((ctx == NULL)) { return DW_PCIE_DM_EINVAL; }
    const uint32_t base = find_ext_cap(ctx, PCIE_EXT_CAP_ID_SPCIE);
    if (base == 0u) { return DW_PCIE_DM_ENOTFOUND; }
    const uint32_t lc3 = cfg_read32(ctx, base + SPCIE_OFF_LINK_CONTROL3);
    cfg_write32(ctx, base + SPCIE_OFF_LINK_CONTROL3, lc3 | SPCIE_LC3_PERFORM_EQUALIZATION);
    return DW_PCIE_DM_OK;
}

int32_t dw_pcie_dm_spcie_set_eq_req_interrupt(const dw_pcie_dm_spcie_ctx_t *ctx, bool enable)
{
    if ((ctx == NULL)) { return DW_PCIE_DM_EINVAL; }
    const uint32_t base = find_ext_cap(ctx, PCIE_EXT_CAP_ID_SPCIE);
    if (base == 0u) { return DW_PCIE_DM_ENOTFOUND; }
    uint32_t lc3 = cfg_read32(ctx, base + SPCIE_OFF_LINK_CONTROL3);
    if (enable) { lc3 |= SPCIE_LC3_EQ_REQ_INT_EN; } else { lc3 &= ~SPCIE_LC3_EQ_REQ_INT_EN; }
    cfg_write32(ctx, base + SPCIE_OFF_LINK_CONTROL3, lc3);
    return DW_PCIE_DM_OK;
}

int32_t dw_pcie_dm_spcie_read_lane_error_status(const dw_pcie_dm_spcie_ctx_t *ctx, uint32_t *status)
{
    if ((ctx == NULL) || (status == NULL)) { return DW_PCIE_DM_EINVAL; }
    const uint32_t base = find_ext_cap(ctx, PCIE_EXT_CAP_ID_SPCIE);
    if (base == 0u) { return DW_PCIE_DM_ENOTFOUND; }
    *status = cfg_read32(ctx, base + SPCIE_OFF_LANE_ERR_STATUS);
    return DW_PCIE_DM_OK;
}
