/* Driver: x1/x2 link width with lane reversal and polarity inversion */
#include "keystone_pcie_module_x1_x2_link_width_with_lane_reversal_and_polarity_inversion.h"

int kpm_set_auto_width_downtrain_disable(const struct pcie_core_api *api, bool disable)
{
    uint32_t v = api->read32(REG_LINK_STAT_CTRL_OFF);
    if (disable) v |= LINK_STAT_HW_AUTO_WD_DIS; else v &= ~LINK_STAT_HW_AUTO_WD_DIS;
    api->write32(REG_LINK_STAT_CTRL_OFF, v);
    return 0;
}

uint32_t kpm_get_negotiated_width(const struct pcie_core_api *api)
{
    (void)api;
    return pcie_link_width();
}
