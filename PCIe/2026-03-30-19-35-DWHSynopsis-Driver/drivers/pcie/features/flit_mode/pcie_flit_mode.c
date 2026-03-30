/*
 * SPDX-License-Identifier: MIT
 * Flit Mode implementation
 */
#include "drivers/pcie/features/flit_mode/pcie_flit_mode.h"

static uint8_t pl64g_version_from_hdr(uint32_t hdr)
{ return (uint8_t)((hdr >> 16) & 0xFu); }

pcie_status_t pcie_flit_query(const pcie_dbi_t* ctx, pcie_flit_info_t* info)
{
    if ((ctx == NULL) || (info == NULL)) { return PCIE_STATUS_INVALID_ARG; }

    uint32_t pcie_cap = pcie_find_pci_cap(ctx, PCI_CAP_ID_PCI_EXP);
    if (pcie_cap == 0u) { info->flit_supported=false; info->pl64g_cap_version=0u; info->pl64g_status=0u; return PCIE_STATUS_UNSUPPORTED; }

    uint32_t dev_cap3 = pcie_read32(ctx, pcie_cap + PCIE_CAP_REG_DEV_CAP3);
    info->flit_supported = ((dev_cap3 & DEV_CAP3_FLIT_MODE_SUPP) != 0u);

    uint32_t pl64g = pcie_find_ext_cap(ctx, PCIE_EXTCAP_ID_PL64G);
    if (pl64g != 0u) { uint32_t hdr = pcie_read32(ctx, pl64g + 0x0u); info->pl64g_cap_version = pl64g_version_from_hdr(hdr); info->pl64g_status = pcie_read32(ctx, pl64g + 0xCu); }
    else { info->pl64g_cap_version = 0u; info->pl64g_status = 0u; }

    return PCIE_STATUS_OK;
}

pcie_status_t pcie_flit_enable(pcie_dbi_t* ctx, bool enable, uint32_t timeout_us)
{
    if (ctx == NULL) { return PCIE_STATUS_INVALID_ARG; }
    uint32_t pcie_cap = pcie_find_pci_cap(ctx, PCI_CAP_ID_PCI_EXP);
    if (pcie_cap == 0u) { return PCIE_STATUS_UNSUPPORTED; }

    uint32_t dev_cap3 = pcie_read32(ctx, pcie_cap + PCIE_CAP_REG_DEV_CAP3);
    if ((dev_cap3 & DEV_CAP3_FLIT_MODE_SUPP) == 0u) { return PCIE_STATUS_UNSUPPORTED; }

    uint32_t lc3s3 = pcie_read32(ctx, pcie_cap + PCIE_CAP_REG_LINK_CTRL3_STAT3);
    uint32_t lc3 = lc3s3 & 0xFFFFu;
    if (enable) { lc3 |= LINK_CTRL3_FLIT_MODE_ENABLE_MASK; } else { lc3 &= ~LINK_CTRL3_FLIT_MODE_ENABLE_MASK; }
    uint32_t new_lc3s3 = (lc3s3 & 0xFFFF0000u) | (lc3 & 0xFFFFu);
    pcie_write32(ctx, pcie_cap + PCIE_CAP_REG_LINK_CTRL3_STAT3, new_lc3s3);

    uint32_t waited = 0u;
    while (waited < timeout_us) {
        uint32_t lc2s2 = pcie_read32(ctx, pcie_cap + PCIE_CAP_REG_LINK_CTRL2_STAT2);
        bool active = ((lc2s2 & LINK_STATUS2_FLIT_MODE_ACTIVE_MASK) != 0u);
        if ((enable && active) || (!enable && !active)) { return PCIE_STATUS_OK; }
        pcie_hal_udelay(10u); waited += 10u;
    }
    return PCIE_STATUS_TIMEOUT;
}

pcie_status_t pcie_flit_status(const pcie_dbi_t* ctx, bool* flit_active, uint32_t* pl64g_status_out)
{
    if ((ctx == NULL) || (flit_active == NULL)) { return PCIE_STATUS_INVALID_ARG; }
    uint32_t pcie_cap = pcie_find_pci_cap(ctx, PCI_CAP_ID_PCI_EXP);
    if (pcie_cap == 0u) { return PCIE_STATUS_UNSUPPORTED; }

    uint32_t lc2s2 = pcie_read32(ctx, pcie_cap + PCIE_CAP_REG_LINK_CTRL2_STAT2);
    *flit_active = ((lc2s2 & LINK_STATUS2_FLIT_MODE_ACTIVE_MASK) != 0u);

    if (pl64g_status_out != NULL) {
        uint32_t pl64g = pcie_find_ext_cap(ctx, PCIE_EXTCAP_ID_PL64G);
        if (pl64g != 0u) { *pl64g_status_out = pcie_read32(ctx, pl64g + 0xCu); }
        else { *pl64g_status_out = 0u; }
    }
    return PCIE_STATUS_OK;
}
