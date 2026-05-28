/* Driver: Link training control and LTSSM monitoring */
#include "keystone_pcie_module_link_training_control_and_ltssm_monitoring.h"

int kpm_ltssm_enable(const struct pcie_core_api *api, bool enable)
{
    uint32_t v = api->read32(REG_CMD_STATUS_OFF);
    if (enable) v |= CMD_STATUS_LTSSM_EN; else v &= ~CMD_STATUS_LTSSM_EN;
    api->write32(REG_CMD_STATUS_OFF, v);
    return 0;
}

int kpm_wait_link_up(const struct pcie_core_api *api, uint32_t timeout_loops)
{
    /* Wait for training=0 and DLL active=1 */
    if (api->poll_bits_all_clear(REG_LINK_STAT_CTRL_OFF, LINK_STAT_TRAINING, timeout_loops))
        return -1;
    if (api->poll_bits_all_set(REG_LINK_STAT_CTRL_OFF, LINK_STAT_DLL_ACTIVE, timeout_loops))
        return -1;
    return 0;
}
