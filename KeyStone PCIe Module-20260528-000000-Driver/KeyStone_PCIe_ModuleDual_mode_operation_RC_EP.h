#ifndef KEYSTONE_PCIE_MODULE_DUAL_MODE_OPERATION_RC_EP_H
#define KEYSTONE_PCIE_MODULE_DUAL_MODE_OPERATION_RC_EP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "core_KeyStone_PCIe_Module.h"

/* Registers and bitfields for Dual-mode operation (unknown => not supported here) */
#define KPM_MODE_REG_OFFSET          (KPM_REG_UNKNOWN)
#define KPM_MODE_MASK                (KPM_REG_UNKNOWN)
#define KPM_MODE_RC_VAL              (KPM_REG_UNKNOWN)
#define KPM_MODE_EP_VAL              (KPM_REG_UNKNOWN)

#define KPM_LTSSM_CTRL_OFFSET        (KPM_REG_UNKNOWN)
#define KPM_LTSSM_EN_BIT             (KPM_REG_UNKNOWN)

#define KPM_DEBUG_STATE_OFFSET       (KPM_REG_UNKNOWN)
#define KPM_DEBUG_LTSSM_STATE_MASK   (KPM_REG_UNKNOWN)
#define KPM_DEBUG_STATE_L0_VAL       (KPM_REG_UNKNOWN)

typedef enum {
    KPM_PCIE_MODE_RC = 0,
    KPM_PCIE_MODE_EP = 1,
} kpm_pcie_mode_t;

int kpm_dualmode_init(const kpm_core_api_t *core);
int kpm_dualmode_set_mode(const kpm_core_api_t *core, kpm_pcie_mode_t mode);
int kpm_dualmode_get_mode(const kpm_core_api_t *core, kpm_pcie_mode_t *mode);
int kpm_dualmode_enable_ltssm(const kpm_core_api_t *core);
int kpm_dualmode_disable_ltssm(const kpm_core_api_t *core);
int kpm_dualmode_get_link_state(const kpm_core_api_t *core, uint32_t *state_raw, bool *in_l0);
int kpm_dualmode_wait_l0(const kpm_core_api_t *core, uint32_t max_iters);

#ifdef __cplusplus
}
#endif

#endif /* KEYSTONE_PCIE_MODULE_DUAL_MODE_OPERATION_RC_EP_H */
