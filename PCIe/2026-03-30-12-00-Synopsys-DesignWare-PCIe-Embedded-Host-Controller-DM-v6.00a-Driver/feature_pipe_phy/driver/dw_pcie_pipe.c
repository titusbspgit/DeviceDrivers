/* SPDX-License-Identifier: BSD-3-Clause */
/* RCI-ag-DrvGen Agent: Synopsys DWC PCIe DM v6.00a — PIPE PHY Interface */

#include "../include/dw_pcie_pipe.h"

static inline uint32_t rd32(const volatile uint8_t *b, uint32_t o){ return dw_mmio_read32_pl(b,o);} 
static inline void wr32(volatile uint8_t *b, uint32_t o, uint32_t v){ dw_mmio_write32_pl(b,o,v);} 

int32_t dw_pcie_pipe_init(dw_pcie_pipe_ctx_t * ctx, volatile void * dbi_base)
{
    if ((ctx == 0) || (dbi_base == 0))
    {
        return -1;
    }
    ctx->dbi_base = (volatile uint8_t *)dbi_base;
    return 0;
}

static int32_t vp_wait_idle(volatile uint8_t * base, uint32_t timeout_loops)
{
    uint32_t loops = 0u;
    while (loops < timeout_loops)
    {
        const uint32_t v = rd32(base, DW_PCIE_PHY_VIEWPORT_CTLSTS_OFF);
        if ((v & DW_PHY_VP_BUSY) == 0u)
        {
            return 0;
        }
        loops++;
    }
    return -2;
}

int32_t dw_pcie_phy_write(dw_pcie_pipe_ctx_t * ctx, uint16_t addr, uint32_t data, uint32_t timeout_loops)
{
    if (ctx == 0)
    {
        return -1;
    }
    volatile uint8_t * base = ctx->dbi_base;
    if (vp_wait_idle(base, timeout_loops) != 0)
    {
        return -2;
    }
    wr32(base, DW_PCIE_PHY_VIEWPORT_DATA_OFF, data);
    uint32_t ctl = ((uint32_t)addr << DW_PHY_VP_ADDR_SHIFT) & DW_PHY_VP_ADDR_MASK;
    ctl |= DW_PHY_VP_RW | DW_PHY_VP_START;
    wr32(base, DW_PCIE_PHY_VIEWPORT_CTLSTS_OFF, ctl);
    return vp_wait_idle(base, timeout_loops);
}

int32_t dw_pcie_phy_read(dw_pcie_pipe_ctx_t * ctx, uint16_t addr, uint32_t * data_out, uint32_t timeout_loops)
{
    if ((ctx == 0) || (data_out == 0))
    {
        return -1;
    }
    volatile uint8_t * base = ctx->dbi_base;
    if (vp_wait_idle(base, timeout_loops) != 0)
    {
        return -2;
    }
    uint32_t ctl = ((uint32_t)addr << DW_PHY_VP_ADDR_SHIFT) & DW_PHY_VP_ADDR_MASK;
    ctl |= DW_PHY_VP_START; /* read */
    wr32(base, DW_PCIE_PHY_VIEWPORT_CTLSTS_OFF, ctl);
    if (vp_wait_idle(base, timeout_loops) != 0)
    {
        return -3;
    }
    *data_out = rd32(base, DW_PCIE_PHY_VIEWPORT_DATA_OFF);
    return 0;
}

bool dw_pcie_phy_ready(const dw_pcie_pipe_ctx_t * ctx)
{
    if (ctx == 0) { return false; }
    const uint32_t s = rd32(ctx->dbi_base, DW_PCIE_PHY_STATUS_OFF);
    return ((s & DW_PHY_STATUS_READY) != 0u);
}

void dw_pcie_pipe_config_msg_bus(dw_pcie_pipe_ctx_t * ctx, uint8_t rx_wb_depth, uint8_t tx_min_wb_depth)
{
    if (ctx == 0) { return; }
    uint32_t v = 0u;
    v |= ((uint32_t)(rx_wb_depth & 0x0Fu));
    v |= ((uint32_t)(tx_min_wb_depth & 0x0Fu)) << 4;
    wr32(ctx->dbi_base, DW_PCIE_PIPE_RELATED_OFF, v);
}
