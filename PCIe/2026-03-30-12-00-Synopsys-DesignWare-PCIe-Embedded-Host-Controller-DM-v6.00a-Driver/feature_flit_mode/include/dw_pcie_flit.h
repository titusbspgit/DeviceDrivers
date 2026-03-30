/* SPDX-License-Identifier: BSD-3-Clause */
/* RCI-ag-DrvGen Agent: Synopsys DWC PCIe DM v6.00a — PCIe 6.0 Flit Mode driver */
#ifndef DW_PCIE_FLIT_H
#define DW_PCIE_FLIT_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Design notes:
 * - This header defines a minimal HAL-style API to control/query PCIe 6.0 Flit Mode
 *   on Synopsys DesignWare PCIe Embedded Host Controller (DM) v6.00a.
 * - Access is via memory-mapped Config Space (ECAM) for PCIe Capability/ExtCaps
 *   and via DBI/Port-Logic base for vendor blocks if needed. No dynamic memory used.
 * - Assumptions (documented; verify against your configured IP manual):
 *   - Extended Capability IDs:
 *       PL64G_EXT_CAP_ID = 0x0031 (Physical Layer 64.0 GT/s)
 *       DEV3_EXT_CAP_ID  = 0x0028 (Device Capabilities 3/Control 3/Status 3)
 *   - PCIe Capability relative offsets (A = PCIe Cap base):
 *       LINK_CAPABILITIES3_OFF = 0x34
 *       LINK_CTRLSTAT3_OFF     = 0x38  (lower 16 bits: Control3, upper 16 bits: Status3)
 *   - Device Capabilities 3/Control 3/Status 3 (B = DEV3 ExtCap base):
 *       DEV3_CAP_OFF   = 0x04
 *       DEV3_CTRL_OFF  = 0x08
 *       DEV3_STAT_OFF  = 0x0C
 *   - Bit fields (conservative placeholders; check exact positions for your IP):
 *       DEV3_CAP_FLIT_SUPPORT       = (1u << 0)
 *       DEV3_CTRL_FLIT_ENABLE       = (1u << 0)
 *       LINK_CTRL3_REQ_L0P          = (1u << 0)   // Request L0p entry (Flit handshake)
 *       LINK_STAT3_L0P_ACTIVE       = (1u << 16)  // Status bit in upper 16-bits region
 *       LINK_STAT3_FLIT_ACTIVE      = (1u << 17)  // Status bit in upper 16-bits region
 */

#define DW_PCIE_FLIT_PL64G_EXT_CAP_ID   (0x0031u)
#define DW_PCIE_FLIT_DEV3_EXT_CAP_ID    (0x0028u)

#define DW_PCIE_CAP_LINK_CAP3_OFF       (0x34u)
#define DW_PCIE_CAP_LINK_CTRLSTAT3_OFF  (0x38u)

#define DW_PCIE_DEV3_CAP_OFF            (0x04u)
#define DW_PCIE_DEV3_CTRL_OFF           (0x08u)
#define DW_PCIE_DEV3_STAT_OFF           (0x0Cu)

#define DW_PCIE_DEV3_CAP_FLIT_SUPPORT   (1u << 0)
#define DW_PCIE_DEV3_CTRL_FLIT_ENABLE   (1u << 0)

#define DW_PCIE_LC3_REQ_L0P             (1u << 0)
#define DW_PCIE_LS3_L0P_ACTIVE          (1u << 16)
#define DW_PCIE_LS3_FLIT_ACTIVE         (1u << 17)

/* Context describing a single Root Port Function */
typedef struct dw_pcie_flit_ctx
{
    volatile uint8_t * ecam_base;   /* ECAM-mapped config space for this function */
    volatile uint8_t * dbi_base;    /* DBI/Port-Logic base (optional) */
    uint32_t            pcie_cap_off;/* Offset of PCIe Capability within ECAM */
    uint32_t            dev3_off;    /* Offset of DEV3 ExtCap within ECAM (0 if absent) */
    uint32_t            pl64g_off;   /* Offset of PL64G ExtCap within ECAM (0 if absent) */
} dw_pcie_flit_ctx_t;

/* Low-level register access (32-bit, little-endian). Offsets must be 4-byte aligned. */
static inline uint32_t dw_mmio_read32(volatile const uint8_t * base, uint32_t off)
{
    const volatile uint32_t * p = (const volatile uint32_t *)(base + off);
    return *p; /* Volatile guarantees single, ordered access */
}

static inline void dw_mmio_write32(volatile uint8_t * base, uint32_t off, uint32_t val)
{
    volatile uint32_t * p = (volatile uint32_t *)(base + off);
    *p = val; /* Volatile guarantees single, ordered access */
}

/* Capability search helpers (Extended Capability space starts at 0x100) */
uint32_t dw_pcie_find_extcap(volatile const uint8_t * ecam_base, uint16_t cap_id);

/* Initialization and discovery */
int32_t dw_pcie_flit_init(dw_pcie_flit_ctx_t * ctx,
                          volatile void * ecam_base,
                          volatile void * dbi_base,
                          uint32_t pcie_cap_off);

int32_t dw_pcie_flit_discover(dw_pcie_flit_ctx_t * ctx);

/* Capability/status */
bool    dw_pcie_flit_supported(const dw_pcie_flit_ctx_t * ctx);
uint32_t dw_pcie_flit_link_status3(const dw_pcie_flit_ctx_t * ctx);
bool    dw_pcie_flit_active(const dw_pcie_flit_ctx_t * ctx);

/* Control APIs */
int32_t dw_pcie_flit_enable(dw_pcie_flit_ctx_t * ctx, uint32_t timeout_loops);
int32_t dw_pcie_flit_disable(dw_pcie_flit_ctx_t * ctx, uint32_t timeout_loops);

#ifdef __cplusplus
}
#endif

#endif /* DW_PCIE_FLIT_H */
