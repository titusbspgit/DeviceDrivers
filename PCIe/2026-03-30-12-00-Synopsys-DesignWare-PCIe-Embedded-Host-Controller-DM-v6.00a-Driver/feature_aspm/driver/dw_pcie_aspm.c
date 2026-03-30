/* SPDX-License-Identifier: BSD-3-Clause */
/* RCI-ag-DrvGen Agent: Synopsys DWC PCIe DM v6.00a — ASPM L0s/L1 Control */

#include "../include/dw_pcie_aspm.h"

int32_t dw_pcie_aspm_init(dw_pcie_aspm_ctx_t * ctx, volatile void * ecam_base, volatile void * dbi_base, uint32_t pcie_cap_off)
{
    if ((ctx == 0) || (ecam_base == 0)) { return -1; }
    ctx->ecam_base    = (volatile uint8_t *)ecam_base;
    ctx->dbi_base     = (volatile uint8_t *)dbi_base;
    ctx->pcie_cap_off = pcie_cap_off;
    return 0;
}

uint32_t dw_pcie_aspm_link_cap(const dw_pcie_aspm_ctx_t * ctx)
{
    return rd32_aspm(ctx->ecam_base, ctx->pcie_cap_off + DW_PCIE_CAP_LINK_CAP_OFF) & DW_PCIE_LCAP_ASPM_MASK;
}

int32_t dw_pcie_aspm_set(const dw_pcie_aspm_ctx_t * ctx, bool l0s, bool l1)
{
    if (ctx == 0) { return -1; }
    uint32_t v = rd32_aspm(ctx->ecam_base, ctx->pcie_cap_off + DW_PCIE_CAP_LINK_CTRLSTS_OFF);
    v &= ~(DW_PCIE_LC_ASPM_L0S_EN | DW_PCIE_LC_ASPM_L1_EN);
    if (l0s) { v |= DW_PCIE_LC_ASPM_L0S_EN; }
    if (l1)  { v |= DW_PCIE_LC_ASPM_L1_EN; }
    wr32_aspm(ctx->ecam_base, ctx->pcie_cap_off + DW_PCIE_CAP_LINK_CTRLSTS_OFF, v);
    return 0;
}

void dw_pcie_aspm_tune(dw_pcie_aspm_ctx_t * ctx, bool extended_synch, uint8_t l1_timer_code)
{
    if (ctx == 0) { return; }
    if (extended_synch)
    {
        uint32_t plc = rd32_aspm(ctx->dbi_base, DW_PCIE_PORT_LINK_CTRL_OFF);
        plc |= DW_PCIE_PORT_EXTENDED_SYNCH;
        wr32_aspm(ctx->dbi_base, DW_PCIE_PORT_LINK_CTRL_OFF, plc);
    }
    uint32_t ackf = rd32_aspm(ctx->dbi_base, DW_PCIE_ACK_F_ASPM_CTRL_OFF);
    ackf |= DW_PCIE_ASPM_L1_TIMER_ENABLE;
    /* Program timer code into bits [29:27] as assumption */
    ackf &= ~(0x7u << 27);
    ackf |= ((uint32_t)(l1_timer_code & 0x7u)) << 27;
    wr32_aspm(ctx->dbi_base, DW_PCIE_ACK_F_ASPM_CTRL_OFF, ackf);
}
