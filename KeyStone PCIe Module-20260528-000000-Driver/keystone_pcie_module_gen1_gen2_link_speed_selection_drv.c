/* Driver: Gen1/Gen2 link speed selection */
#include "keystone_pcie_module_gen1_gen2_link_speed_selection.h"

int kpm_set_target_link_speed(const struct pcie_core_api *api, kpm_link_speed_t gen)
{
    uint32_t v = api->read32(REG_LINK_CTRL2_OFF);
    v &= ~LINK_CTRL2_TGT_SPEED_MASK;
    switch (gen) {
    case KPM_LINK_GEN1: v |= LINK_CTRL2_TGT_SPEED_GEN1; break;
    case KPM_LINK_GEN2: v |= LINK_CTRL2_TGT_SPEED_GEN2; break;
    default: return -1;
    }
    api->write32(REG_LINK_CTRL2_OFF, v);
    return 0;
}

int kpm_retrain_link_and_wait(const struct pcie_core_api *api, uint32_t timeout_loops)
{
    /* Write-1-set to retrain */
    api->write32(REG_LINK_STAT_CTRL_OFF, LINK_STAT_RETRAIN_LINK);
    /* Wait for training to clear and DLL active */
    if (api->poll_bits_all_clear(REG_LINK_STAT_CTRL_OFF, LINK_STAT_TRAINING, timeout_loops))
        return -1;
    if (api->poll_bits_all_set(REG_LINK_STAT_CTRL_OFF, LINK_STAT_DLL_ACTIVE, timeout_loops))
        return -1;
    return 0;
}

uint32_t kpm_get_negotiated_speed(const struct pcie_core_api *api)
{
    (void)api;
    return pcie_link_speed();
}
