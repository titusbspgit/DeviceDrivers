/* Driver: MSI generation (EP mode) */
#include "keystone_pcie_module_msi_generation_ep_mode.h"

int kpm_msi_program_target(const struct pcie_core_api *api, uint32_t addr_low, uint32_t addr_up, uint16_t data)
{
    api->write32(REG_MSI_ADDR_LOW_OFF, addr_low);
    api->write32(REG_MSI_ADDR_UP_OFF, addr_up);
    uint32_t v = (uint32_t)data;
    api->write32(REG_MSI_DATA_OFF, v);
    return 0;
}
