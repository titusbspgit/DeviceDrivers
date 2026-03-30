/* SPDX-License-Identifier: BSD-3-Clause */
/* RCI-ag-DrvGen Agent: Synopsys DWC PCIe DM v6.00a — Secondary PCIe ExtCap (SPCIE) */
#ifndef DW_PCIE_SPCIE_H
#define DW_PCIE_SPCIE_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DW_PCIE_SPCIE_EXT_CAP_ID   (0x0019u)

/* Offsets from SPCIE base */
#define DW_SPCIE_CAP_HEADER_OFF    (0x00u)
#define DW_SPCIE_LINK_CONTROL3_OFF (0x04u)
#define DW_SPCIE_LANE_ERR_STS_OFF  (0x08u)

/* Fields (assumptions based on spec) */
#define DW_SPCIE_LC3_PERFORM_EQ    (1u << 0) /* RW1S: request link EQ */

/* Lane error status W1C bits: [W-1:0] */

/* Context */
typedef struct dw_pcie_spcie_ctx
{
    volatile uint8_t * ecam_base;
    uint32_t            spcie_off; /* 0 if not present */
} dw_pcie_spcie_ctx_t;

uint32_t dw_pcie_find_extcap(volatile const uint8_t * ecam_base, uint16_t cap_id); /* from flit header; duplicate decl ok */

int32_t  dw_pcie_spcie_init(dw_pcie_spcie_ctx_t * ctx, volatile void * ecam_base);
int32_t  dw_pcie_spcie_discover(dw_pcie_spcie_ctx_t * ctx);
bool     dw_pcie_spcie_present(const dw_pcie_spcie_ctx_t * ctx);

int32_t  dw_pcie_spcie_request_equalization(dw_pcie_spcie_ctx_t * ctx);
uint32_t dw_pcie_spcie_get_lane_err_status(const dw_pcie_spcie_ctx_t * ctx);
int32_t  dw_pcie_spcie_clear_lane_err_status(dw_pcie_spcie_ctx_t * ctx, uint32_t mask);

#ifdef __cplusplus
}
#endif

#endif /* DW_PCIE_SPCIE_H */
