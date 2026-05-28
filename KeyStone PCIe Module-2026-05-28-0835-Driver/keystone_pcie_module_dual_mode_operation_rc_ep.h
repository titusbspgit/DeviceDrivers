#ifndef KEYSTONE_PCIE_MODULE_DUAL_MODE_OPERATION_RC_EP_H
#define KEYSTONE_PCIE_MODULE_DUAL_MODE_OPERATION_RC_EP_H

#include <stdint.h>
#include <stdbool.h>
#include "core_keystone_pcie_module.h"

/* Registers and bitfields for Dual-mode operation (UNKNOWN if not provided) */
#define KPM_REG_DEVSTAT_OFFSET            0xFFFFFFFFu /* UNKNOWN */
#define KPM_DEVSTAT_PCIESSMODE_MASK       0x00000003u /* UNKNOWN */
#define KPM_DEVSTAT_PCIESSMODE_SHIFT      0u          /* UNKNOWN */

/* Return codes */
#define KPM_OK                 (0)
#define KPM_ERR_INVAL          (-1)
#define KPM_ERR_UNSUPPORTED    (-2)

typedef enum {
    KPM_ROLE_RC = 0,
    KPM_ROLE_EP = 1
} KPM_DualModeRole;

#ifdef __cplusplus
extern "C" {
#endif

int kpm_dual_mode_init(const KPM_CoreApi *api);
int kpm_dual_mode_set_role(const KPM_CoreApi *api, KPM_DualModeRole role);
int kpm_dual_mode_get_role(const KPM_CoreApi *api, KPM_DualModeRole *role);

#ifdef __cplusplus
}
#endif

#endif /* KEYSTONE_PCIE_MODULE_DUAL_MODE_OPERATION_RC_EP_H */
