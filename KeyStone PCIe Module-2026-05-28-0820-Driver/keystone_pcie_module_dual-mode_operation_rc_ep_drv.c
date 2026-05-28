#include "keystone_pcie_module_dual-mode_operation_rc_ep.h"

kpm_status_t kpm_pcie_set_mode(const kpm_core_api_t *api, kpm_pcie_mode_t mode)
{
    if (!api) return KPM_STATUS_INVALID_ARG;
    api->mb();
    (void)mode;
    return KPM_STATUS_NOT_IMPLEMENTED; /* Registers UNKNOWN */
}

kpm_status_t kpm_pcie_get_mode(const kpm_core_api_t *api, kpm_pcie_mode_t *mode_out)
{
    if (!api || !mode_out) return KPM_STATUS_INVALID_ARG;
    api->mb();
    *mode_out = KPM_PCIE_MODE_UNKNOWN;
    return KPM_STATUS_NOT_IMPLEMENTED; /* Registers UNKNOWN */
}

kpm_status_t kpm_pcie_apply_mode(const kpm_core_api_t *api)
{
    if (!api) return KPM_STATUS_INVALID_ARG;
    api->mb();
    return KPM_STATUS_NOT_IMPLEMENTED; /* Registers UNKNOWN */
}
