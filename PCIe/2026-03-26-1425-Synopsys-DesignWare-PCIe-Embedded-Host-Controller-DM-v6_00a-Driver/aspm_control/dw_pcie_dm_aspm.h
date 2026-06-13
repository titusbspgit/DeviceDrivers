/*
 * Synopsys DesignWare PCIe (DM) v6.00a - ASPM L0s/L1 Control
 * Header
 */
#ifndef DW_PCIE_DM_ASPM_H
#define DW_PCIE_DM_ASPM_H

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

/* Context for ASPM control via PF0 DBI */
typedef struct {
    volatile uint8_t *dbi_base;
    volatile uint8_t *dsp_base; /* optional vendor DSP base for ACK/ASPM knobs */
} dw_pcie_dm_aspm_ctx_t;

/* Standard PCIe Capability IDs */
#define PCIE_CAP_ID_PCI_EXPRESS        (0x10u)

/* Offsets within the PCI Express Capability */
#define PCIE_CAP_OFF_PTR               (0x34u) /* in config header */
#define PCIE_CAP_OFF_LINK_CONTROL      (0x10u) /* offset from PCIe cap base */

/* ASPM control encodings in Link Control [1:0] */
typedef enum {
    ASPM_DISABLE = 0u,
    ASPM_L0S = 1u,
    ASPM_L1 = 2u,
    ASPM_L0S_L1 = 3u
} dw_pcie_aspm_mode_t;

/* Vendor DSP optional registers */
#define DWC_ACK_F_ASPM_CTRL_OFF  (0x70Cu)
#define DWC_PORT_LINK_CTRL_OFF   (0x710u)

/* Public API */
int32_t dw_pcie_dm_aspm_init(dw_pcie_dm_aspm_ctx_t *ctx, uintptr_t dbi_base, uintptr_t dsp_base /* may be 0 */);
int32_t dw_pcie_dm_aspm_set_mode(const dw_pcie_dm_aspm_ctx_t *ctx, dw_pcie_aspm_mode_t mode);
int32_t dw_pcie_dm_aspm_get_mode(const dw_pcie_dm_aspm_ctx_t *ctx, dw_pcie_aspm_mode_t *mode);
int32_t dw_pcie_dm_aspm_vendor_rmw(const dw_pcie_dm_aspm_ctx_t *ctx, uint32_t reg_off, uint32_t mask, uint32_t value);

#ifdef __cplusplus
}
#endif

#endif /* DW_PCIE_DM_ASPM_H */
