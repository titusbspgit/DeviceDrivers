/* SPDX-License-Identifier: BSD-3-Clause */
/* RCI-ag-DrvGen Agent: Synopsys DWC PCIe DM v6.00a — PCIe 6.0 Flit Mode driver */

#include "../include/dw_pcie_flit.h"

#define DW_PCIE_EXTCAP_START   (0x100u)

/* Extended Capability header fields */
#define EXT_CAP_ID_MASK        (0x0000FFFFu)
#define EXT_CAP_NEXT_MASK      (0xFFF00000u)
#define EXT_CAP_NEXT_SHIFT     (20u)

static inline uint32_t rd32(const volatile uint8_t *b, uint32_t o){ return dw_mmio_read32(b,o);} 
static inline void wr32(volatile uint8_t *b, uint32_t o, uint32_t v){ dw_mmio_write32(b,o,v);} 

uint32_t dw_pcie_find_extcap(volatile const uint8_t * ecam_base, uint16_t cap_id)
{
    uint32_t off = DW_PCIE_EXTCAP_START;
    while (off != 0u)
    {
        const uint32_t hdr = rd32(ecam_base, off);
        const uint16_t id  = (uint16_t)(hdr & EXT_CAP_ID_MASK);
        if (id == cap_id)
        {
            return off;
        }
        const uint32_t next = (hdr & EXT_CAP_NEXT_MASK) >> EXT_CAP_NEXT_SHIFT;
        if (next == 0u)
        {
            return 0u; /* Not found */
        }
        off = next;
    }
    return 0u;
}

int32_t dw_pcie_flit_init(dw_pcie_flit_ctx_t * ctx,
                          volatile void * ecam_base,
                          volatile void * dbi_base,
                          uint32_t pcie_cap_off)
{
    if ((ctx == 0) || (ecam_base == 0))
    {
        return -1;
    }
    ctx->ecam_base   = (volatile uint8_t *)ecam_base;
    ctx->dbi_base    = (volatile uint8_t *)dbi_base;
    ctx->pcie_cap_off= pcie_cap_off;
    ctx->dev3_off    = 0u;
    ctx->pl64g_off   = 0u;
    return 0;
}

int32_t dw_pcie_flit_discover(dw_pcie_flit_ctx_t * ctx)
{
    if (ctx == 0)
    {
        return -1;
    }
    /* Discover DEV3 and PL64G ext caps */
    ctx->dev3_off  = dw_pcie_find_extcap(ctx->ecam_base, (uint16_t)DW_PCIE_FLIT_DEV3_EXT_CAP_ID);
    ctx->pl64g_off = dw_pcie_find_extcap(ctx->ecam_base, (uint16_t)DW_PCIE_FLIT_PL64G_EXT_CAP_ID);
    return 0;
}

bool dw_pcie_flit_supported(const dw_pcie_flit_ctx_t * ctx)
{
    if ((ctx == 0) || (ctx->dev3_off == 0u))
    {
        return false;
    }
    const uint32_t cap = rd32(ctx->ecam_base, ctx->dev3_off + DW_PCIE_DEV3_CAP_OFF);
    return ((cap & DW_PCIE_DEV3_CAP_FLIT_SUPPORT) != 0u);
}

uint32_t dw_pcie_flit_link_status3(const dw_pcie_flit_ctx_t * ctx)
{
    const uint32_t v = rd32(ctx->ecam_base, ctx->pcie_cap_off + DW_PCIE_CAP_LINK_CTRLSTAT3_OFF);
    return v; /* Upper 16 bits treated as status per assumption */
}

bool dw_pcie_flit_active(const dw_pcie_flit_ctx_t * ctx)
{
    const uint32_t v = dw_pcie_flit_link_status3(ctx);
    return ((v & DW_PCIE_LS3_FLIT_ACTIVE) != 0u);
}

int32_t dw_pcie_flit_enable(dw_pcie_flit_ctx_t * ctx, uint32_t timeout_loops)
{
    if ((ctx == 0) || (ctx->dev3_off == 0u))
    {
        return -1;
    }
    if (!dw_pcie_flit_supported(ctx))
    {
        return -2; /* Not supported */
    }
    /* 1) Enable Flit Mode in Device Control 3 */
    uint32_t dev3_ctrl = rd32(ctx->ecam_base, ctx->dev3_off + DW_PCIE_DEV3_CTRL_OFF);
    dev3_ctrl |= DW_PCIE_DEV3_CTRL_FLIT_ENABLE;
    wr32(ctx->ecam_base, ctx->dev3_off + DW_PCIE_DEV3_CTRL_OFF, dev3_ctrl);

    /* 2) Request L0p entry to initiate handshake */
    uint32_t lc3 = rd32(ctx->ecam_base, ctx->pcie_cap_off + DW_PCIE_CAP_LINK_CTRLSTAT3_OFF);
    lc3 |= DW_PCIE_LC3_REQ_L0P;
    wr32(ctx->ecam_base, ctx->pcie_cap_off + DW_PCIE_CAP_LINK_CTRLSTAT3_OFF, lc3);

    /* 3) Poll for FLIT active */
    uint32_t loops = 0u;
    while (loops < timeout_loops)
    {
        const uint32_t ls3 = rd32(ctx->ecam_base, ctx->pcie_cap_off + DW_PCIE_CAP_LINK_CTRLSTAT3_OFF);
        if ((ls3 & DW_PCIE_LS3_FLIT_ACTIVE) != 0u)
        {
            return 0; /* Success */
        }
        loops++;
    }
    return -3; /* Timeout */
}

int32_t dw_pcie_flit_disable(dw_pcie_flit_ctx_t * ctx, uint32_t timeout_loops)
{
    if ((ctx == 0) || (ctx->dev3_off == 0u))
    {
        return -1;
    }
    /* 1) Clear L0p request */
    uint32_t lc3 = rd32(ctx->ecam_base, ctx->pcie_cap_off + DW_PCIE_CAP_LINK_CTRLSTAT3_OFF);
    lc3 &= (~DW_PCIE_LC3_REQ_L0P);
    wr32(ctx->ecam_base, ctx->pcie_cap_off + DW_PCIE_CAP_LINK_CTRLSTAT3_OFF, lc3);

    /* 2) Optionally clear Flit Enable */
    uint32_t dev3_ctrl = rd32(ctx->ecam_base, ctx->dev3_off + DW_PCIE_DEV3_CTRL_OFF);
    dev3_ctrl &= (~DW_PCIE_DEV3_CTRL_FLIT_ENABLE);
    wr32(ctx->ecam_base, ctx->dev3_off + DW_PCIE_DEV3_CTRL_OFF, dev3_ctrl);

    /* 3) Poll for FLIT inactive */
    uint32_t loops = 0u;
    while (loops < timeout_loops)
    {
        const uint32_t ls3 = rd32(ctx->ecam_base, ctx->pcie_cap_off + DW_PCIE_CAP_LINK_CTRLSTAT3_OFF);
        if ((ls3 & DW_PCIE_LS3_FLIT_ACTIVE) == 0u)
        {
            return 0; /* Success */
        }
        loops++;
    }
    return -3; /* Timeout */
}
