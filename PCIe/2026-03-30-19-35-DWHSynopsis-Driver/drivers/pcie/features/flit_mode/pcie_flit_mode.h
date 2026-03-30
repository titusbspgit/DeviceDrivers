/*
 * SPDX-License-Identifier: MIT
 * PCIe 6.0 Flit Mode feature
 */
#ifndef PCIE_FLIT_MODE_H
#define PCIE_FLIT_MODE_H

#include "drivers/pcie/common/pcie_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef PCIE_CFG_LINK_CTRL3_FLIT_MODE_ENABLE
#define PCIE_CFG_LINK_CTRL3_FLIT_MODE_ENABLE (13u)
#endif
#ifndef PCIE_CFG_LINK_STATUS2_FLIT_MODE_ACTIVE
#define PCIE_CFG_LINK_STATUS2_FLIT_MODE_ACTIVE (26u)
#endif

#define LINK_CTRL3_FLIT_MODE_ENABLE_MASK   (1u << PCIE_CFG_LINK_CTRL3_FLIT_MODE_ENABLE)
#define LINK_STATUS2_FLIT_MODE_ACTIVE_MASK (1u << PCIE_CFG_LINK_STATUS2_FLIT_MODE_ACTIVE)

typedef struct
{
    bool     flit_supported;
    uint8_t  pl64g_cap_version;
    uint32_t pl64g_status;
} pcie_flit_info_t;

pcie_status_t pcie_flit_query(const pcie_dbi_t* ctx, pcie_flit_info_t* info);
pcie_status_t pcie_flit_enable(pcie_dbi_t* ctx, bool enable, uint32_t timeout_us);
pcie_status_t pcie_flit_status(const pcie_dbi_t* ctx, bool* flit_active, uint32_t* pl64g_status_out);

#ifdef __cplusplus
}
#endif

#endif /* PCIE_FLIT_MODE_H */
