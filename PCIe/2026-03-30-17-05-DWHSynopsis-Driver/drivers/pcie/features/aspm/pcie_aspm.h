/*
 * MIT License
 * ASPM L0s/L1 Control
 */
#ifndef PCIE_ASPM_H
#define PCIE_ASPM_H

#include "drivers/pcie/common/pcie_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    PCIE_ASPM_DISABLED = 0,
    PCIE_ASPM_L0S      = 1,
    PCIE_ASPM_L1       = 2,
    PCIE_ASPM_L0S_L1   = 3
} pcie_aspm_policy_t;

#define ACK_F_ASPM_CTRL_L1_TIMER_EN      (1u << 31)
#define ACK_F_ASPM_CTRL_L1_LAT_POS       (27u)
#define ACK_F_ASPM_CTRL_L1_LAT_MASK      (0x7u << ACK_F_ASPM_CTRL_L1_LAT_POS)
#define ACK_F_ASPM_CTRL_L0S_LAT_POS      (24u)
#define ACK_F_ASPM_CTRL_L0S_LAT_MASK     (0x7u << ACK_F_ASPM_CTRL_L0S_LAT_POS)
#define ACK_F_ASPM_CTRL_COMMONCLK_NFTS_POS (16u)
#define ACK_F_ASPM_CTRL_COMMONCLK_NFTS_MASK (0xFFu << ACK_F_ASPM_CTRL_COMMONCLK_NFTS_POS)
#define ACK_F_ASPM_CTRL_ACK_NFTS_POS     (8u)
#define ACK_F_ASPM_CTRL_ACK_NFTS_MASK    (0xFFu << ACK_F_ASPM_CTRL_ACK_NFTS_POS)
#define ACK_F_ASPM_CTRL_ACK_FREQ_POS     (0u)
#define ACK_F_ASPM_CTRL_ACK_FREQ_MASK    (0xFFu << ACK_F_ASPM_CTRL_ACK_FREQ_POS)

pcie_status_t pcie_aspm_set_policy(pcie_dbi_t* ctx, pcie_aspm_policy_t policy);
pcie_status_t pcie_aspm_get_policy(const pcie_dbi_t* ctx, pcie_aspm_policy_t* out_policy);
pcie_status_t pcie_aspm_tune_ack_params(pcie_dbi_t* ctx,
                                        uint8_t ack_freq, uint8_t ack_nfts, uint8_t commonclk_nfts,
                                        uint8_t l0s_lat_sel, uint8_t l1_lat_sel,
                                        bool enable_l1_timer);

#ifdef __cplusplus
}
#endif

#endif /* PCIE_ASPM_H */
