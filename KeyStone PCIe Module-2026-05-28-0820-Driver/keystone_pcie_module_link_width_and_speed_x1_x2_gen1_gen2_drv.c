#include "keystone_pcie_module_link_width_and_speed_x1_x2_gen1_gen2.h"

kpm_status_t kpm_pcie_link_configure(const kpm_core_api_t *api, const kpm_pcie_link_cfg_t *cfg)
{
    if (!api || !cfg) return KPM_STATUS_INVALID_ARG;
    api->mb();
    (void)cfg;
    return KPM_STATUS_NOT_IMPLEMENTED; /* Registers UNKNOWN */
}

kpm_status_t kpm_pcie_link_get_status(const kpm_core_api_t *api, kpm_pcie_link_cfg_t *status_out)
{
    if (!api || !status_out) return KPM_STATUS_INVALID_ARG;
    api->mb();
    status_out->width = KPM_PCIE_LINK_WIDTH_X1;
    status_out->gen = KPM_PCIE_GEN1;
    return KPM_STATUS_NOT_IMPLEMENTED; /* Registers UNKNOWN */
}
