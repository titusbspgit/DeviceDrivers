/*
 * SPDX-License-Identifier: MIT
 * PIPE PHY Interface implementation
 */
#include "drivers/pcie/features/pipe_phy/pcie_pipe_phy.h"

static pcie_status_t wait_not_busy(const pcie_dbi_t* ctx, const pcie_phy_bus_cfg_t* cfg, uint32_t timeout_us)
{
    uint32_t waited = 0u;
    while ((pcie_read32(ctx, cfg->ctlsts_off) & cfg->busy_bit) != 0u) {
        if (waited >= timeout_us) { return PCIE_STATUS_TIMEOUT; }
        pcie_hal_udelay(1u); waited += 1u;
    }
    return PCIE_STATUS_OK;
}

pcie_status_t pcie_phy_read(const pcie_dbi_t* ctx, const pcie_phy_bus_cfg_t* cfg,
                            uint16_t reg_addr, uint16_t* out_val, uint8_t lane, bool broadcast,
                            uint32_t timeout_us)
{
    if ((ctx == NULL) || (cfg == NULL) || (out_val == NULL)) { return PCIE_STATUS_INVALID_ARG; }
    pcie_status_t st = wait_not_busy(ctx, cfg, timeout_us); if (st != PCIE_STATUS_OK) { return st; }

    uint32_t ctl = 0u;
    uint32_t addrf = (((uint32_t)reg_addr & cfg->addr_mask) << cfg->addr_shift);
    ctl |= addrf;
    if (cfg->lane_mask != 0u) { ctl |= (((uint32_t)lane << cfg->lane_shift) & cfg->lane_mask); }
    if ((cfg->broadcast_bit != 0u) && broadcast) { ctl |= cfg->broadcast_bit; }
    ctl |= cfg->go_bit;
    pcie_write32((pcie_dbi_t*)ctx, cfg->ctlsts_off, ctl);

    st = wait_not_busy(ctx, cfg, timeout_us); if (st != PCIE_STATUS_OK) { return st; }

    uint32_t data = pcie_read32(ctx, cfg->data_off);
    *out_val = (uint16_t)(data & 0xFFFFu);
    return PCIE_STATUS_OK;
}

pcie_status_t pcie_phy_write(pcie_dbi_t* ctx, const pcie_phy_bus_cfg_t* cfg,
                             uint16_t reg_addr, uint16_t val, uint8_t lane, bool broadcast,
                             uint32_t timeout_us)
{
    if ((ctx == NULL) || (cfg == NULL)) { return PCIE_STATUS_INVALID_ARG; }
    pcie_status_t st = wait_not_busy(ctx, cfg, timeout_us); if (st != PCIE_STATUS_OK) { return st; }

    pcie_write32(ctx, cfg->data_off, (uint32_t)val & 0xFFFFu);

    uint32_t ctl = 0u;
    uint32_t addrf = (((uint32_t)reg_addr & cfg->addr_mask) << cfg->addr_shift);
    ctl |= addrf;
    if (cfg->lane_mask != 0u) { ctl |= (((uint32_t)lane << cfg->lane_shift) & cfg->lane_mask); }
    if ((cfg->broadcast_bit != 0u) && broadcast) { ctl |= cfg->broadcast_bit; }
    ctl |= cfg->write_bit; ctl |= cfg->go_bit;
    pcie_write32(ctx, cfg->ctlsts_off, ctl);

    st = wait_not_busy(ctx, cfg, timeout_us); return st;
}

pcie_status_t pcie_phy_basic_init(pcie_dbi_t* ctx, const pcie_phy_bus_cfg_t* cfg)
{
    if ((ctx == NULL) || (cfg == NULL)) { return PCIE_STATUS_INVALID_ARG; }
    uint16_t v = 0u; return pcie_phy_read(ctx, cfg, 0u, &v, 0u, true, 2000u);
}
