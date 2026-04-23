/*
 * Synopsys DWC PCIe DM v6.00a - External PIPE PHY Linux shim
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef DW_PCIE_PIPE_PHY_H
#define DW_PCIE_PIPE_PHY_H

#include <linux/types.h>
#include <linux/device.h>
#include <linux/regmap.h>
#include <linux/clk.h>
#include <linux/reset.h>
#include <linux/phy/phy.h>
#include <linux/platform_device.h>

#include "dw_pcie_pipe_common.h"

struct dw_pcie_pipe_phy_cfg {
    u32 prb_timeout_us;
    u32 refclk_hz;
    bool ssc_enable;
    u8 pipe_gen;
};

struct dw_pcie_pipe_phy;

/* Vendor ops (weakly linked, optional) */
struct dw_pcie_pipe_vendor_ops {
    int (*set_refclk_ssc)(struct dw_pcie_pipe_phy *ctx, bool ssc_enable, u32 refclk_hz);
    int (*program_pipe_gen)(struct dw_pcie_pipe_phy *ctx, u8 pipe_gen);
    int (*assert_resets)(struct dw_pcie_pipe_phy *ctx);
    int (*deassert_resets)(struct dw_pcie_pipe_phy *ctx);
    int (*is_pll_locked)(struct dw_pcie_pipe_phy *ctx, bool *locked);
};

struct dw_pcie_pipe_phy {
    struct device *dev;
    void __iomem *base;
    struct clk *refclk;
    struct reset_control *rst_pma;
    struct reset_control *rst_pipe;
    struct phy *ext_phy; /* optional Linux PHY framework handle */

    struct dw_pcie_pipe_phy_cfg policy;
    const struct dw_pcie_pipe_vendor_ops *vops; /* may be NULL */
};

/* PRB access */
int dw_pcie_prb_read(struct dw_pcie_pipe_phy *ctx, u16 prb_addr, u32 *data_out);
int dw_pcie_prb_write(struct dw_pcie_pipe_phy *ctx, u16 prb_addr, u32 data_in);

/* Capability dump */
int dw_pcie_pipe_dump_caps(struct dw_pcie_pipe_phy *ctx,
                           u8 *tx_min_wr_buf_depth,
                           u8 *rx_wr_buf_depth,
                           bool *garbage_data_mode);

#endif /* DW_PCIE_PIPE_PHY_H */
