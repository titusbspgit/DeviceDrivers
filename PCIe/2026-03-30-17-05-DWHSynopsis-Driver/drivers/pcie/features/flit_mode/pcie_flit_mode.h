/*
 * MIT License
 * PCIe 6.0 Flit Mode feature
 */
#ifndef PCIE_FLIT_MODE_H
#define PCIE_FLIT_MODE_H

#include "drivers/pcie/common/pcie_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Configurable bit positions via config header (override at build if needed) */
#ifndef PCIE_CFG_LINK_CTRL3_FLIT_MODE_ENABLE
#define PCIE_CFG_LINK_CTRL3_FLIT_MODE_ENABLE (13u)
#endif
#ifndef PCIE_CFG_LINK_STATUS2_FLIT_MODE_ACTIVE
#define PCIE_CFG_LINK_STATUS2_FLIT_MODE_ACTIVE (26u)
#endif

#define LINK_CTRL3_FLIT_MODE_ENABLE_MASK  (1u << PCIE_CFG_LINK_CTRL3_FLIT_MODE_ENABLE)
#define LINK_STATUS2_FLIT_MODE_ACTIVE_MASK (1u << PCIE_CFG_LINK_STATUS2_FLIT_MODE_ACTIVE)

typedef struct
{
    bool     flit_supported;
    uint8_t  pl64g_cap_version; /* CAP_VERSION from PL64G header [19:16], 0 if absent */
    uint32_t pl64g_status;      /* Raw PL64G STATUS dword if present */
} pcie_flit_info_t;

/* Discover FLIT capability and PL64G presence */
pcie_status_t pcie_flit_query(const pcie_dbi_t* ctx, pcie_flit_info_t* info);

/* Enable or disable FLIT mode (negotiation as per PCIe 6.0) */
pcie_status_t pcie_flit_enable(pcie_dbi_t* ctx, bool enable, uint32_t timeout_us);

/* Read current FLIT operational status */
pcie_status_t pcie_flit_status(const pcie_dbi_t* ctx, bool* flit_active, uint32_t* pl64g_status_out);

#ifdef __cplusplus
}
#endif

#endif /* PCIE_FLIT_MODE_H */
