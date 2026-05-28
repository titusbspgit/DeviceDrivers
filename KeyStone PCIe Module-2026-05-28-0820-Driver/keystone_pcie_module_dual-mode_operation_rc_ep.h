#ifndef KEYSTONE_PCIE_MODULE_DUAL_MODE_OPERATION_RC_EP_H
#define KEYSTONE_PCIE_MODULE_DUAL_MODE_OPERATION_RC_EP_H

#include <stdint.h>
#include <stdbool.h>
#include "core_keystone_pcie_module.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Registers for mode control are UNKNOWN in current spec. */
/* Example placeholders (do not use):
 * #define KPM_DEVSTAT_OFFSET /* UNKNOWN */
 */

typedef enum {
    KPM_PCIE_MODE_UNKNOWN = 0,
    KPM_PCIE_MODE_RC = 1,
    KPM_PCIE_MODE_EP = 2,
} kpm_pcie_mode_t;

kpm_status_t kpm_pcie_set_mode(const kpm_core_api_t *api, kpm_pcie_mode_t mode);

kpm_status_t kpm_pcie_get_mode(const kpm_core_api_t *api, kpm_pcie_mode_t *mode_out);

kpm_status_t kpm_pcie_apply_mode(const kpm_core_api_t *api);

#ifdef __cplusplus
}
#endif

#endif /* KEYSTONE_PCIE_MODULE_DUAL_MODE_OPERATION_RC_EP_H */
