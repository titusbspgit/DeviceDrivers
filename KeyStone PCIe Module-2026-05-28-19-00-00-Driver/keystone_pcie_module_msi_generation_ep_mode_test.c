#include "core_keystone_pcie_module.h"
#include "keystone_pcie_module_msi_generation_ep_mode.h"
int main(void){ const pcie_core_ops_t* ops=pcie_core_get_ops(); kpm_msi_tx_init(ops); kpm_msi_tx_fire(ops,0); return 0; }
