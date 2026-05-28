#include "core_keystone_pcie_module.h"
#include "keystone_pcie_module_link_training_control_and_ltssm_monitoring.h"
int main(void){ const pcie_core_ops_t* ops=pcie_core_get_ops(); kpm_ltssm_init(ops); kpm_ltssm_enable(ops,true); return 0; }
