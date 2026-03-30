/*
 * SPDX-License-Identifier: MIT
 * ASPM control implementation
 */
#include "drivers/pcie/features/aspm/pcie_aspm.h"

pcie_status_t pcie_aspm_set_policy(pcie_dbi_t* ctx, pcie_aspm_policy_t policy)
{
    if (ctx == NULL) { return PCIE_STATUS_INVALID_ARG; }
    uint32_t pcie_cap = pcie_find_pci_cap(ctx, PCI_CAP_ID_PCI_EXP);
    if (pcie_cap == 0u) { return PCIE_STATUS_UNSUPPORTED; }

    uint32_t lc_off = pcie_cap + 0x10u;
    uint16_t link_control = pcie_read16(ctx, lc_off);
    link_control = (uint16_t)((link_control & (uint16_t)~0x3u) | ((uint16_t)policy & 0x3u));
    pcie_write16(ctx, lc_off, link_control);
    return PCIE_STATUS_OK;
}

pcie_status_t pcie_aspm_get_policy(const pcie_dbi_t* ctx, pcie_aspm_policy_t* out_policy)
{
    if ((ctx == NULL) || (out_policy == NULL)) { return PCIE_STATUS_INVALID_ARG; }
    uint32_t pcie_cap = pcie_find_pci_cap(ctx, PCI_CAP_ID_PCI_EXP);
    if (pcie_cap == 0u) { return PCIE_STATUS_UNSUPPORTED; }

    uint32_t lc_off = pcie_cap + 0x10u; uint16_t link_control = pcie_read16(ctx, lc_off);
    *out_policy = (pcie_aspm_policy_t)(link_control & 0x3u); return PCIE_STATUS_OK;
}

pcie_status_t pcie_aspm_tune_ack_params(pcie_dbi_t* ctx,
                                        uint8_t ack_freq, uint8_t ack_nfts, uint8_t commonclk_nfts,
                                        uint8_t l0s_lat_sel, uint8_t l1_lat_sel,
                                        bool enable_l1_timer)
{
    if (ctx == NULL) { return PCIE_STATUS_INVALID_ARG; }
    uint32_t v = 0u;
    v |= ((uint32_t)ack_freq << ACK_F_ASPM_CTRL_ACK_FREQ_POS) & ACK_F_ASPM_CTRL_ACK_FREQ_MASK;
    v |= ((uint32_t)ack_nfts << ACK_F_ASPM_CTRL_ACK_NFTS_POS) & ACK_F_ASPM_CTRL_ACK_NFTS_MASK;
    v |= ((uint32_t)commonclk_nfts << ACK_F_ASPM_CTRL_COMMONCLK_NFTS_POS) & ACK_F_ASPM_CTRL_COMMONCLK_NFTS_MASK;
    v |= ((uint32_t)l0s_lat_sel << ACK_F_ASPM_CTRL_L0S_LAT_POS) & ACK_F_ASPM_CTRL_L0S_LAT_MASK;
    v |= ((uint32_t)l1_lat_sel << ACK_F_ASPM_CTRL_L1_LAT_POS) & ACK_F_ASPM_CTRL_L1_LAT_MASK;
    if (enable_l1_timer) { v |= ACK_F_ASPM_CTRL_L1_TIMER_EN; }
    pcie_write32(ctx, 0x70Cu, v); return PCIE_STATUS_OK;
}
