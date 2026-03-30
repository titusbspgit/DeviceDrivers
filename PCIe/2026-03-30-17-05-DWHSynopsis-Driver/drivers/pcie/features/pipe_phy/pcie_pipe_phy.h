/*
 * MIT License
 * PIPE PHY Interface via DWC PHY Viewport (External/Custom PHY)
 */
#ifndef PCIE_PIPE_PHY_H
#define PCIE_PIPE_PHY_H

#include "drivers/pcie/common/pcie_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    uint32_t ctlsts_off;      /* default: DWC_PHY_VIEWPORT_CTLSTS_OFF */
    uint32_t data_off;        /* default: DWC_PHY_VIEWPORT_DATA_OFF */
    uint32_t addr_mask;       /* lower address bits used by PHY (default 0xFFFF) */
    uint8_t  addr_shift;      /* default 0 */
    uint32_t go_bit;          /* write-1-to-start bit mask (default (1u<<31)) */
    uint32_t write_bit;       /* 1=write, 0=read (default (1u<<30)) */
    uint32_t busy_bit;        /* status busy bit mask (default (1u<<29)) */
    uint32_t lane_mask;       /* optional lane index field mask (0 if N/A) */
    uint8_t  lane_shift;      /* shift for lane index */
    uint32_t broadcast_bit;   /* optional broadcast to all lanes (0 if N/A) */
} pcie_phy_bus_cfg_t;

#define PCIE_PHY_BUS_CFG_DEFAULT  { \
    DWC_PHY_VIEWPORT_CTLSTS_OFF, DWC_PHY_VIEWPORT_DATA_OFF, \
    0x0000FFFFu, 0u, (1u<<31), (1u<<30), (1u<<29), 0u, 0u, 0u }

pcie_status_t pcie_phy_read(const pcie_dbi_t* ctx, const pcie_phy_bus_cfg_t* cfg,
                            uint16_t reg_addr, uint16_t* out_val, uint8_t lane, bool broadcast,
                            uint32_t timeout_us);

pcie_status_t pcie_phy_write(pcie_dbi_t* ctx, const pcie_phy_bus_cfg_t* cfg,
                             uint16_t reg_addr, uint16_t val, uint8_t lane, bool broadcast,
                             uint32_t timeout_us);

pcie_status_t pcie_phy_basic_init(pcie_dbi_t* ctx, const pcie_phy_bus_cfg_t* cfg);

#ifdef __cplusplus
}
#endif

#endif /* PCIE_PIPE_PHY_H */
