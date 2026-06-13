/*
 * Synopsys DesignWare PCIe (DM) v6.00a - Link Training and Equalization Controls (Vendor DSP)
 * Header
 */
#ifndef DW_PCIE_DM_LINK_EQ_H
#define DW_PCIE_DM_LINK_EQ_H

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

/* Context: base address of DWC_PCIE_DSP port logic block (vendor registers) */
typedef struct {
    volatile uint8_t *dsp_base; /* Must map vendor port logic block */
} dw_pcie_dm_linkeq_ctx_t;

/* Known offsets (relative to dsp_base) per Synopsys documentation */
#define DWC_VENDOR_DLLP_OFF      (0x704u)
#define DWC_PORT_FORCE_OFF       (0x708u)
#define DWC_ACK_F_ASPM_CTRL_OFF  (0x70Cu)
#define DWC_PORT_LINK_CTRL_OFF   (0x710u)
#define DWC_GEN3_RELATED_OFF     (0x890u)
#define DWC_GEN3_EQ_LOCAL_FS_LF  (0x894u)

/* Public API (generic masked RMW to avoid relying on variant-specific bitfields) */
int32_t dw_pcie_dm_linkeq_init(dw_pcie_dm_linkeq_ctx_t *ctx, uintptr_t dsp_base);
int32_t dw_pcie_dm_linkeq_rmw(const dw_pcie_dm_linkeq_ctx_t *ctx, uint32_t reg_off, uint32_t mask, uint32_t value);
int32_t dw_pcie_dm_linkeq_write_local_fs_lf(const dw_pcie_dm_linkeq_ctx_t *ctx, uint8_t fs, uint8_t lf);
int32_t dw_pcie_dm_linkeq_port_force(const dw_pcie_dm_linkeq_ctx_t *ctx, uint32_t mask, uint32_t value);

#ifdef __cplusplus
}
#endif

#endif /* DW_PCIE_DM_LINK_EQ_H */
