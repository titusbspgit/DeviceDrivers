/*
 * MIT License
 * SPCIE implementation
 */
#include "drivers/pcie/features/spcie/pcie_spcie.h"

pcie_status_t pcie_spcie_get_link3(const pcie_dbi_t* ctx, pcie_spcie_link3_t* out)
{
    if ((ctx == NULL) || (out == NULL)) { return PCIE_STATUS_INVALID_ARG; }
    uint32_t off = pcie_find_ext_cap(ctx, PCIE_EXTCAP_ID_SPCIE);
    if (off == 0u) { return PCIE_STATUS_UNSUPPORTED; }
    uint32_t v = pcie_read32(ctx, off + 0x04u);
    out->link_control3 = (uint16_t)(v & 0xFFFFu);
    out->link_status3  = (uint16_t)((v >> 16) & 0xFFFFu);
    return PCIE_STATUS_OK;
}

pcie_status_t pcie_spcie_set_link_control3(pcie_dbi_t* ctx, uint16_t link_control3)
{
    if (ctx == NULL) { return PCIE_STATUS_INVALID_ARG; }
    uint32_t off = pcie_find_ext_cap(ctx, PCIE_EXTCAP_ID_SPCIE);
    if (off == 0u) { return PCIE_STATUS_UNSUPPORTED; }
    uint32_t v = pcie_read32(ctx, off + 0x04u);
    v = (v & 0xFFFF0000u) | ((uint32_t)link_control3 & 0xFFFFu);
    pcie_write32(ctx, off + 0x04u, v);
    return PCIE_STATUS_OK;
}

pcie_status_t pcie_spcie_get_lane_err_status(const pcie_dbi_t* ctx, uint32_t* out_mask)
{
    if ((ctx == NULL) || (out_mask == NULL)) { return PCIE_STATUS_INVALID_ARG; }
    uint32_t off = pcie_find_ext_cap(ctx, PCIE_EXTCAP_ID_SPCIE);
    if (off == 0u) { return PCIE_STATUS_UNSUPPORTED; }
    *out_mask = pcie_read32(ctx, off + 0x08u);
    return PCIE_STATUS_OK;
}

pcie_status_t pcie_spcie_clear_lane_err_status(pcie_dbi_t* ctx, uint32_t clear_mask)
{
    if (ctx == NULL) { return PCIE_STATUS_INVALID_ARG; }
    uint32_t off = pcie_find_ext_cap(ctx, PCIE_EXTCAP_ID_SPCIE);
    if (off == 0u) { return PCIE_STATUS_UNSUPPORTED; }
    pcie_write32(ctx, off + 0x08u, clear_mask); /* RW1C */
    return PCIE_STATUS_OK;
}
