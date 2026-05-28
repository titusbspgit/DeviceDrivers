/* Driver: Common clock and SSC configuration */
#include "keystone_pcie_module_common_clock_and_ssc_configuration.h"

int kpm_set_common_clock(const struct pcie_core_api *api, bool enable)
{
    uint32_t v = api->read32(REG_LINK_STAT_CTRL_OFF);
    if (enable) v |= LINK_STAT_COMMON_CLK_CFG; else v &= ~LINK_STAT_COMMON_CLK_CFG;
    api->write32(REG_LINK_STAT_CTRL_OFF, v);
    return 0;
}

bool kpm_get_common_clock(const struct pcie_core_api *api)
{
    uint32_t v = api->read32(REG_LINK_STAT_CTRL_OFF);
    return (v & LINK_STAT_COMMON_CLK_CFG) != 0u;
}
