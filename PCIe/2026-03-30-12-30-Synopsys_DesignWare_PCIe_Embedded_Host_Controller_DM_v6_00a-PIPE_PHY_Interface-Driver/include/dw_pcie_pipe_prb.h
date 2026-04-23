/*
 * Synopsys DesignWare PCIe DM v6.00a - PHY Register Bus helpers
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef DW_PCIE_PIPE_PRB_H
#define DW_PCIE_PIPE_PRB_H

#include "dw_pcie_pipe_common.h"

static inline u32 dw_pcie_prb_ctl_build(u16 addr, bool is_write)
{
    u32 v = 0u;
    v |= ((u32)addr & DW_PCIE_PRB_CTLSTS_ADDR_MASK) << DW_PCIE_PRB_CTLSTS_ADDR_SHIFT;
    if (is_write) {
        v |= DW_PCIE_PRB_CTLSTS_RDNWR;
    }
    return v;
}

#endif /* DW_PCIE_PIPE_PRB_H */
