#include "core_keystone_pcie_module.h"
#include "keystone_pcie_module_link_speed_gen1_gen2_selection.h"
int main(void){ const pcie_core_ops_t* ops=pcie_core_get_ops(); kpm_link_speed_init(ops); kpm_set_gen2(ops,true); return 0; }
