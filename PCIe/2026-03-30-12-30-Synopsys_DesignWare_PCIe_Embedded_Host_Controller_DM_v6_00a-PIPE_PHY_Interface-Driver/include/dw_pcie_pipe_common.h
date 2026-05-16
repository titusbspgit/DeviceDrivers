/*
 * Common macros for DWC PCIe DM v6.00a PIPE/PRB
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef DW_PCIE_PIPE_COMMON_H
#define DW_PCIE_PIPE_COMMON_H

#include <linux/types.h>

/* Port Logic (DBI/ELBI) offsets per RAG */
#define DW_PCIE_PHY_VIEWPORT_CTLSTS_OFF       (0x0B70u)
#define DW_PCIE_PHY_VIEWPORT_DATA_OFF         (0x0B74u)
#define DW_PCIE_PIPE_RELATED_OFF              (0x0B90u)
#define DW_PCIE_PORT_LOGIC_CTRL2_OFF          (0x0B94u)
#define DW_PCIE_PIPE_LOOPBACK_CONTROL_OFF     (0x08B8u)
#define DW_PCIE_MULTI_LANE_CONTROL_OFF        (0x08C0u)
#define DW_PCIE_MISC_CONTROL_1_DBIRWEN_OFF    (0x08BCu)
#define DW_PCIE_SD_STATUS_L3_OFF              (0x00C4u)

/* PRB CTLSTS layout — concrete per RAG; guarded by CONFIG_DW_PCIE_PRB_ASSUMED
 * If your RM differs, override via Kconfig or headers before including this.
 */
#ifndef CONFIG_DW_PCIE_PRB_ASSUMED
#define CONFIG_DW_PCIE_PRB_ASSUMED 1
#endif

#if CONFIG_DW_PCIE_PRB_ASSUMED
#define DW_PCIE_PRB_CTLSTS_ADDR_MASK          (0x0000FFFFu)
#define DW_PCIE_PRB_CTLSTS_ADDR_SHIFT         (0u)
#define DW_PCIE_PRB_CTLSTS_RDNWR              (1u << 16)
#define DW_PCIE_PRB_CTLSTS_ERROR              (1u << 29)
#define DW_PCIE_PRB_CTLSTS_TIMEOUT            (1u << 30)
#define DW_PCIE_PRB_CTLSTS_GO_BUSY            (1u << 31)
#endif

#define DW_PCIE_PIPE_RELATED_TX_MSG_MIN_WR_BUF_DEPTH_MASK   (0x000000F0u)
#define DW_PCIE_PIPE_RELATED_TX_MSG_MIN_WR_BUF_DEPTH_SHIFT  (4u)
#define DW_PCIE_PIPE_RELATED_RX_MSG_WR_BUF_DEPTH_MASK       (0x0000000Fu)
#define DW_PCIE_PIPE_RELATED_RX_MSG_WR_BUF_DEPTH_SHIFT      (0u)
#define DW_PCIE_PIPE_RELATED_GARBAGE_DATA_MODE              (1u << 8)

#define DW_PCIE_PRB_BUSY_TIMEOUT_US_DEFAULT   (10000u)
#define DW_PCIE_PRB_BUSY_POLL_STEP_US         (10u)

#endif /* DW_PCIE_PIPE_COMMON_H */
