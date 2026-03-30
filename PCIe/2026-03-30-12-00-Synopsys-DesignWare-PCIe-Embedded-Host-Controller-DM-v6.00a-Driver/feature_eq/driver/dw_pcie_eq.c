/* SPDX-License-Identifier: BSD-3-Clause */
/* RCI-ag-DrvGen Agent: Synopsys DWC PCIe DM v6.00a — Link Training & EQ Controls */

#include "../include/dw_pcie_eq.h"

int32_t dw_pcie_eq_init(dw_pcie_eq_ctx_t * ctx, volatile void * dbi_base)
{
    if ((ctx == 0) || (dbi_base == 0)) { return -1; }
    ctx->dbi_base = (volatile uint8_t *)dbi_base;
    return 0;
}

void dw_pcie_eq_set_local_fs_lf(dw_pcie_eq_ctx_t * ctx, uint8_t fs, uint8_t lf)
{
    if (ctx == 0) { return; }
    uint32_t v = 0u;
    v |= ((uint32_t)(lf & DW_PCIE_GEN3_EQ_LOCAL_LF_MASK));
    v |= ((uint32_t)(fs & DW_PCIE_GEN3_EQ_LOCAL_FS_MASK)) << DW_PCIE_GEN3_EQ_LOCAL_FS_SHIFT;
    wr32_eq(ctx->dbi_base, DW_PCIE_GEN3_EQ_LOCAL_FS_LF_OFF, v);
}

void dw_pcie_eq_get_local_fs_lf(const dw_pcie_eq_ctx_t * ctx, uint8_t * fs, uint8_t * lf)
{
    if ((ctx == 0) || (fs == 0) || (lf == 0)) { return; }
    const uint32_t v = rd32_eq(ctx->dbi_base, DW_PCIE_GEN3_EQ_LOCAL_FS_LF_OFF);
    *lf = (uint8_t)(v & DW_PCIE_GEN3_EQ_LOCAL_LF_MASK);
    *fs = (uint8_t)((v >> DW_PCIE_GEN3_EQ_LOCAL_FS_SHIFT) & DW_PCIE_GEN3_EQ_LOCAL_FS_MASK);
}

int32_t dw_pcie_eq_set_preset(dw_pcie_eq_ctx_t * ctx, uint8_t preset_index, uint8_t pre, uint8_t cur, uint8_t post)
{
    if ((ctx == 0) || (preset_index > 10u)) { return -1; }
    wr32_eq(ctx->dbi_base, DW_PCIE_GEN3_EQ_PSET_INDEX_OFF, (uint32_t)preset_index);
    uint32_t v = 0u;
    v |= ((uint32_t)(pre  & DW_PCIE_GEN3_EQ_PSET_PRE_MASK));
    v |= ((uint32_t)(cur  & DW_PCIE_GEN3_EQ_PSET_CUR_MASK))  << DW_PCIE_GEN3_EQ_PSET_CUR_SHIFT;
    v |= ((uint32_t)(post & DW_PCIE_GEN3_EQ_PSET_POST_MASK)) << DW_PCIE_GEN3_EQ_PSET_POST_SHIFT;
    wr32_eq(ctx->dbi_base, DW_PCIE_GEN3_EQ_PSET_COEF_MAP_OFF, v);
    return 0;
}

int32_t dw_pcie_eq_get_preset(dw_pcie_eq_ctx_t * ctx, uint8_t preset_index, uint8_t * pre, uint8_t * cur, uint8_t * post)
{
    if ((ctx == 0) || (pre == 0) || (cur == 0) || (post == 0) || (preset_index > 10u)) { return -1; }
    wr32_eq(ctx->dbi_base, DW_PCIE_GEN3_EQ_PSET_INDEX_OFF, (uint32_t)preset_index);
    const uint32_t v = rd32_eq(ctx->dbi_base, DW_PCIE_GEN3_EQ_PSET_COEF_MAP_OFF);
    *pre  = (uint8_t)(v & DW_PCIE_GEN3_EQ_PSET_PRE_MASK);
    *cur  = (uint8_t)((v >> DW_PCIE_GEN3_EQ_PSET_CUR_SHIFT) & DW_PCIE_GEN3_EQ_PSET_CUR_MASK);
    *post = (uint8_t)((v >> DW_PCIE_GEN3_EQ_PSET_POST_SHIFT) & DW_PCIE_GEN3_EQ_PSET_POST_MASK);
    return 0;
}

int32_t dw_pcie_send_vendor_dllp(dw_pcie_eq_ctx_t * ctx, uint32_t payload, uint32_t timeout_loops)
{
    if (ctx == 0) { return -1; }
    wr32_eq(ctx->dbi_base, DW_PCIE_VENDOR_SPEC_DLLP_OFF, payload);
    /* Start */
    uint32_t ctl = rd32_eq(ctx->dbi_base, DW_PCIE_VENDOR_SPEC_DLLP_OFF);
    ctl |= DW_PCIE_DLLP_START;
    wr32_eq(ctx->dbi_base, DW_PCIE_VENDOR_SPEC_DLLP_OFF, ctl);
    /* Bounded wait for busy to clear */
    uint32_t loops = 0u;
    while (loops < timeout_loops)
    {
        const uint32_t v = rd32_eq(ctx->dbi_base, DW_PCIE_VENDOR_SPEC_DLLP_OFF);
        if ((v & DW_PCIE_DLLP_BUSY) == 0u) { return 0; }
        loops++;
    }
    return -2;
}

void dw_pcie_force_retrain(dw_pcie_eq_ctx_t * ctx)
{
    if (ctx == 0) { return; }
    uint32_t v = rd32_eq(ctx->dbi_base, DW_PCIE_PORT_FORCE_OFF);
    v |= DW_PCIE_PORT_FORCE_RETRAIN;
    wr32_eq(ctx->dbi_base, DW_PCIE_PORT_FORCE_OFF, v);
}
