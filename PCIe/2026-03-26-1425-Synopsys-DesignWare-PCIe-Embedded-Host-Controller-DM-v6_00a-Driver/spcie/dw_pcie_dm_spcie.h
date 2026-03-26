/*
 * Synopsys DesignWare PCIe (DM) v6.00a - Secondary PCIe Extended Capability (SPCIE)
 * Header
 */
#ifndef DW_PCIE_DM_SPCIE_H
#define DW_PCIE_DM_SPCIE_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    DW_PCIE_DM_OK = 0,
    DW_PCIE_DM_EINVAL = -1,
    DW_PCIE_DM_ENOTSUP = -2,
    DW_PCIE_DM_ENOTFOUND = -3,
    DW_PCIE_DM_EBUSY = -4,
    DW_PCIE_DM_ETIMEOUT = -5
} dw_pcie_dm_status_t;

/* Context */
typedef struct {
    volatile uint8_t *dbi_base; /* PF0 DBI config base */
} dw_pcie_dm_spcie_ctx_t;

/* Capability IDs and offsets */
#define PCIE_EXT_CAP_ID_SPCIE            (0x0019u)
#define SPCIE_OFF_LINK_CONTROL3          (0x04u)
#define SPCIE_OFF_LANE_ERR_STATUS        (0x08u)

/* LINK_CONTROL3 bits */
#define SPCIE_LC3_PERFORM_EQUALIZATION   (1u << 0)
#define SPCIE_LC3_EQ_REQ_INT_EN          (1u << 1)

/* Public API */
int32_t dw_pcie_dm_spcie_init(dw_pcie_dm_spcie_ctx_t *ctx, uintptr_t dbi_base);
int32_t dw_pcie_dm_spcie_is_present(const dw_pcie_dm_spcie_ctx_t *ctx, bool *present);
int32_t dw_pcie_dm_spcie_request_equalization(const dw_pcie_dm_spcie_ctx_t *ctx);
int32_t dw_pcie_dm_spcie_set_eq_req_interrupt(const dw_pcie_dm_spcie_ctx_t *ctx, bool enable);
int32_t dw_pcie_dm_spcie_read_lane_error_status(const dw_pcie_dm_spcie_ctx_t *ctx, uint32_t *status);

#ifdef __cplusplus
}
#endif

#endif /* DW_PCIE_DM_SPCIE_H */
