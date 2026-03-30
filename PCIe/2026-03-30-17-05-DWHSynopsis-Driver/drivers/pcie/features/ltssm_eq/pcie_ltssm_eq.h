/*
 * MIT License
 * Link Training and Equalization Controls (Gen3+)
 */
#ifndef PCIE_LTSSM_EQ_H
#define PCIE_LTSSM_EQ_H

#include "drivers/pcie/common/pcie_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GEN3_EQ_FS_POS   (6u)
#define GEN3_EQ_FS_MASK  (0x3Fu << GEN3_EQ_FS_POS)
#define GEN3_EQ_LF_POS   (0u)
#define GEN3_EQ_LF_MASK  (0x3Fu << GEN3_EQ_LF_POS)

/* Configurable PORT_FORCE bits (override via -D macros if different) */
#ifndef PORT_FORCE_SEND_VS_DLLP
#define PORT_FORCE_SEND_VS_DLLP   (1u << 0)
#endif
#ifndef PORT_FORCE_LTSSM_FORCE_EN
#define PORT_FORCE_LTSSM_FORCE_EN (1u << 8)
#endif
#ifndef PORT_FORCE_LTSSM_STATE_POS
#define PORT_FORCE_LTSSM_STATE_POS (4u)
#endif
#ifndef PORT_FORCE_LTSSM_STATE_MASK
#define PORT_FORCE_LTSSM_STATE_MASK (0xFu << PORT_FORCE_LTSSM_STATE_POS)
#endif

pcie_status_t pcie_eq_set_local_fs_lf(pcie_dbi_t* ctx, uint8_t fs, uint8_t lf);
pcie_status_t pcie_eq_program_preset_map(pcie_dbi_t* ctx, uint32_t index, uint32_t coef_dw);
pcie_status_t pcie_eq_get_status(const pcie_dbi_t* ctx, uint32_t* out_status);
pcie_status_t pcie_port_send_vendor_dllp(pcie_dbi_t* ctx, uint32_t payload);
pcie_status_t pcie_port_force_ltssm_state(pcie_dbi_t* ctx, uint8_t state_code, bool enable_force);

#ifdef __cplusplus
}
#endif

#endif /* PCIE_LTSSM_EQ_H */
