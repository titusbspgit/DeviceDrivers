/*
 * Synopsys DesignWare PCIe (DM) v6.00a - PCIe 6.0 64.0 GT/s FLIT Mode Support (PL64G)
 * Source
 *
 * Notes:
 * - Uses DBI window to scan Extended Capabilities for PL64G (ID 0x0031)
 * - PL64G CONTROL/CAPABILITY are reserved for this IP; only STATUS is consumed
 * - No dynamic allocation; deterministic MMIO access
 */
#include "dw_pcie_dm_pcie6_flit_mode.h"
#include <stdio.h>

/* MMIO helpers */
static inline uint32_t mmio_read32(const volatile void *addr)
{
    const volatile uint32_t *p = (const volatile uint32_t *)addr;
    return DW_PCIE_DM_LE32_TO_CPU(*p);
}

static inline void mmio_write32(volatile void *addr, uint32_t value)
{
    volatile uint32_t *p = (volatile uint32_t *)addr;
    *p = DW_PCIE_DM_CPU_TO_LE32(value);
    (void)*p; /* posted write flush */
}

/* PCIe configuration space helpers */
#define PCIE_EXT_CAP_START  (0x100u)

static uint16_t cfg_read16(const dw_pcie_dm_pl64g_ctx_t *ctx, uint32_t off)
{
    return (uint16_t)(mmio_read32((const void *)(ctx->dbi_base + (off & ~0x3u))) >> ((off & 0x2u) * 8u));
}

static uint32_t cfg_read32(const dw_pcie_dm_pl64g_ctx_t *ctx, uint32_t off)
{
    return mmio_read32((const void *)(ctx->dbi_base + off));
}

static uint32_t find_ext_cap(const dw_pcie_dm_pl64g_ctx_t *ctx, uint16_t cap_id)
{
    uint32_t off = PCIE_EXT_CAP_START;
    uint32_t guard = 0u;
    while ((off != 0u) && (off <= 0xFFC0u) && (guard < 64u)) {
        const uint32_t hdr = cfg_read32(ctx, off + 0x0u);
        const uint16_t id = (uint16_t)(hdr & 0xFFFFu);
        const uint16_t next = (uint16_t)((hdr >> 20) & 0xFFFu);
        if (id == cap_id) {
            return off;
        }
        off = (uint32_t)next;
        guard++;
    }
    return 0u;
}

int32_t dw_pcie_dm_pl64g_init(dw_pcie_dm_pl64g_ctx_t *ctx, uintptr_t dbi_base)
{
    if ((ctx == NULL) || (dbi_base == (uintptr_t)0)) {
        return DW_PCIE_DM_EINVAL;
    }
    ctx->dbi_base = (volatile uint8_t *)dbi_base;
    return DW_PCIE_DM_OK;
}

int32_t dw_pcie_dm_pl64g_is_present(const dw_pcie_dm_pl64g_ctx_t *ctx, bool *present)
{
    if ((ctx == NULL) || (present == NULL)) {
        return DW_PCIE_DM_EINVAL;
    }
    const uint32_t off = find_ext_cap(ctx, PCIE_EXT_CAP_ID_PL64G);
    *present = (off != 0u);
    return DW_PCIE_DM_OK;
}

int32_t dw_pcie_dm_pl64g_read_status(const dw_pcie_dm_pl64g_ctx_t *ctx, uint32_t *status)
{
    if ((ctx == NULL) || (status == NULL)) {
        return DW_PCIE_DM_EINVAL;
    }
    const uint32_t off = find_ext_cap(ctx, PCIE_EXT_CAP_ID_PL64G);
    if (off == 0u) {
        return DW_PCIE_DM_ENOTFOUND;
    }
    *status = cfg_read32(ctx, off + PL64G_OFF_STATUS);
    return DW_PCIE_DM_OK;
}

int32_t dw_pcie_dm_pl64g_wait_eq_complete(const dw_pcie_dm_pl64g_ctx_t *ctx, uint32_t timeout_us, bool *completed)
{
    if ((ctx == NULL) || (completed == NULL)) {
        return DW_PCIE_DM_EINVAL;
    }
    const uint32_t off = find_ext_cap(ctx, PCIE_EXT_CAP_ID_PL64G);
    if (off == 0u) {
        return DW_PCIE_DM_ENOTFOUND;
    }
    uint32_t stat = 0u;
    uint32_t iter = 0u;
    const uint32_t step_us = 10u; /* polling granularity */
    *completed = false;
    while (iter <= timeout_us) {
        stat = cfg_read32(ctx, off + PL64G_OFF_STATUS);
        if ((stat & PL64G_STS_EQ_64G_CPL) != 0u) {
            *completed = true;
            return DW_PCIE_DM_OK;
        }
        /* Busy-wait delay loop for step_us; platform should replace with calibrated delay */
        for (volatile uint32_t d = 0u; d < 100u; ++d) { __asm__ __volatile__("" : : : "memory"); }
        if (timeout_us < step_us) {
            break;
        }
        iter += step_us;
    }
    return DW_PCIE_DM_ETIMEOUT;
}

int32_t dw_pcie_dm_pl64g_precoding_status(const dw_pcie_dm_pl64g_ctx_t *ctx, bool *required, bool *enabled)
{
    if ((ctx == NULL) || (required == NULL) || (enabled == NULL)) {
        return DW_PCIE_DM_EINVAL;
    }
    const uint32_t off = find_ext_cap(ctx, PCIE_EXT_CAP_ID_PL64G);
    if (off == 0u) {
        return DW_PCIE_DM_ENOTFOUND;
    }
    const uint32_t stat = cfg_read32(ctx, off + PL64G_OFF_STATUS);
    *required = ((stat & PL64G_STS_TX_PRECODING_REQ) != 0u);
    *enabled = ((stat & PL64G_STS_TX_PRECODING_ON) != 0u);
    return DW_PCIE_DM_OK;
}
