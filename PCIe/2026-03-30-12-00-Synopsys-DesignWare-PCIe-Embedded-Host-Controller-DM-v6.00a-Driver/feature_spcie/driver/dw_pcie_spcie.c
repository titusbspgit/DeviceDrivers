/* SPDX-License-Identifier: BSD-3-Clause */
/* RCI-ag-DrvGen Agent: Synopsys DWC PCIe DM v6.00a — Secondary PCIe ExtCap (SPCIE) */

#include "../include/dw_pcie_spcie.h"

static inline uint32_t rd32(const volatile uint8_t *b, uint32_t o)
{ const volatile uint32_t *p=(const volatile uint32_t*)(b+o); return *p; }
static inline void wr32(volatile uint8_t *b, uint32_t o, uint32_t v)
{ volatile uint32_t *p=(volatile uint32_t*)(b+o); *p=v; }

int32_t dw_pcie_spcie_init(dw_pcie_spcie_ctx_t * ctx, volatile void * ecam_base)
{
    if ((ctx == 0) || (ecam_base == 0)) { return -1; }
    ctx->ecam_base = (volatile uint8_t *)ecam_base;
    ctx->spcie_off = 0u;
    return 0;
}

int32_t dw_pcie_spcie_discover(dw_pcie_spcie_ctx_t * ctx)
{
    if (ctx == 0) { return -1; }
    ctx->spcie_off = dw_pcie_find_extcap(ctx->ecam_base, (uint16_t)DW_PCIE_SPCIE_EXT_CAP_ID);
    return 0;
}

bool dw_pcie_spcie_present(const dw_pcie_spcie_ctx_t * ctx)
{
    return (ctx != 0) && (ctx->spcie_off != 0u);
}

int32_t dw_pcie_spcie_request_equalization(dw_pcie_spcie_ctx_t * ctx)
{
    if (!dw_pcie_spcie_present(ctx)) { return -1; }
    uint32_t v = rd32(ctx->ecam_base, ctx->spcie_off + DW_SPCIE_LINK_CONTROL3_OFF);
    v |= DW_SPCIE_LC3_PERFORM_EQ;
    wr32(ctx->ecam_base, ctx->spcie_off + DW_SPCIE_LINK_CONTROL3_OFF, v);
    return 0;
}

uint32_t dw_pcie_spcie_get_lane_err_status(const dw_pcie_spcie_ctx_t * ctx)
{
    if (!dw_pcie_spcie_present(ctx)) { return 0u; }
    return rd32(ctx->ecam_base, ctx->spcie_off + DW_SPCIE_LANE_ERR_STS_OFF);
}

int32_t dw_pcie_spcie_clear_lane_err_status(dw_pcie_spcie_ctx_t * ctx, uint32_t mask)
{
    if (!dw_pcie_spcie_present(ctx)) { return -1; }
    /* W1C: write mask to clear */
    wr32(ctx->ecam_base, ctx->spcie_off + DW_SPCIE_LANE_ERR_STS_OFF, mask);
    return 0;
}
