/*
 * Synopsys DesignWare PCIe (DM v6.00a) — PCIe 6.0 64.0 GT/s Flit Mode Feature Driver (Header)
 *
 * RCI-ag-DrvGen Agent
 *
 * Target: Memory-mapped DBI interface; portable C, MISRA-style discipline.
 *
 * Notes:
 * - Accesses DBI configuration space of PF0 via base address provided by caller.
 * - No dynamic allocation. No OS assumptions.
 */
#ifndef DW_PCIE6_FLIT_H
#define DW_PCIE6_FLIT_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Return codes */
#define DW_PCIE6_OK                (0)
#define DW_PCIE6_ERR_PARAM         (-1)
#define DW_PCIE6_ERR_CAP_NOT_FOUND (-2)
#define DW_PCIE6_ERR_UNSUPPORTED   (-3)
#define DW_PCIE6_ERR_TIMEOUT       (-4)

/* Port Logic: DBI RO write enable */
#define PL_MISC_CONTROL_1_OFF   (0x8BCu) /* DWC Port Logic: MISC_CONTROL_1 */
#define PL_MISC_CONTROL_1_DBI_RO_WR_EN (1u << 0)

/* PCI Configuration header offsets (DBI view) */
#define PCI_CFG_CAP_PTR_OFF     (0x34u) /* 8-bit Capability Pointer */

/* PCIe Capability (Cap ID 0x10) relative offsets (B = base of this capability) */
#define PCIE_CAP_ID             (0x10u)
#define PCIE_CAP_LNKCTLSTS_OFF  (0x10u) /* Link Control (15:0) | Link Status (31:16) */
#define PCIE_CAP_LNKCTL2STS2_OFF (0x30u) /* Link Control 2 (15:0) | Link Status 2 (31:16) */
#define PCIE_CAP_DEVCAP3_OFF    (0x34u)
#define PCIE_CAP_DEVCTL3STS3_OFF (0x38u) /* Device Control 3 (15:0) | Device Status 3 (31:16) */

/* Link Control bits */
#define PCIE_LNKCTL_RL          (1u << 5)  /* Retrain Link */

/* Link Status bits */
#define PCIE_LNKSTA_DLLLA       (1u << 13) /* Data Link Layer Link Active */
#define PCIE_LNKSTA_CLS_MASK    (0xFu)     /* Current Link Speed (bits 3:0) */

/* Link Control 2 fields */
#define PCIE_LNKCTL2_TLS_MASK   (0xFu)     /* Target Link Speed, bits [3:0] */
#define PCIE_LINK_SPEED_GEN1    (0x1u)
#define PCIE_LINK_SPEED_GEN2    (0x2u)
#define PCIE_LINK_SPEED_GEN3    (0x3u)
#define PCIE_LINK_SPEED_GEN4    (0x4u)
#define PCIE_LINK_SPEED_GEN5    (0x5u)
#define PCIE_LINK_SPEED_GEN6    (0x6u)     /* 64.0 GT/s per PCIe 6.0 */

/* Device Capabilities 3 bits (exposed when FLIT supported) */
#define PCIE_DEVCAP3_FLIT_MODE_SUPP   (1u << 0)

/* Device Control 3 / Status 3 bits */
#define PCIE_DEVCTL3_ENTER_FLIT_MODE  (1u << 0)
#define PCIE_DEVSTS3_FLIT_MODE_STATUS (1u << 0)

/* Physical Layer 64.0 GT/s Extended Capability */
#define PCIE_EXT_CAP_START       (0x100u)    /* First Extended Capability */
#define PCIE_EXT_CAP_HDR_NEXT_MASK (0xFFF00000u)
#define PCIE_EXT_CAP_HDR_NEXT_SHIFT (20u)
#define PCIE_EXT_CAP_HDR_ID_MASK (0x0000FFFFu)
#define PCIE_EXT_CAP_ID_PL64G    (0x0031u)

/* Driver context */
typedef struct dw_pcie6_ctx {
    volatile uint8_t *dbi_base;   /* DBI base (PF0) */
    uint32_t pcie_cap_off;        /* Cached base of PCIe Capability (Cap ID 0x10). 0 if not probed */
    uint32_t pl64g_cap_off;       /* Cached base of PL64G Ext Cap. 0 if not found */
    void (*udelay)(uint32_t usec); /* Optional microsecond delay hook */
} dw_pcie6_t;

/* Public API */
int dw_pcie6_init(dw_pcie6_t *ctx, uintptr_t dbi_base_addr);
void dw_pcie6_set_udelay(dw_pcie6_t *ctx, void (*udelay_cb)(uint32_t));
int dw_pcie6_find_caps(dw_pcie6_t *ctx);
int dw_pcie6_enable_flit_64g(dw_pcie6_t *ctx, uint32_t timeout_us);
bool dw_pcie6_is_flit_supported(const dw_pcie6_t *ctx);
int dw_pcie6_get_status(dw_pcie6_t *ctx, bool *flit_mode, uint8_t *cur_speed, bool *dl_active);

#ifdef __cplusplus
}
#endif

#endif /* DW_PCIE6_FLIT_H */
