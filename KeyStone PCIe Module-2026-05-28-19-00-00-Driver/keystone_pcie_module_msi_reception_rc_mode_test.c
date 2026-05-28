#include "core_keystone_pcie_module.h"
#include "keystone_pcie_module_msi_reception_rc_mode.h"
int main(void){ const pcie_core_ops_t* ops=pcie_core_get_ops(); kpm_msi_rx_init(ops); kpm_msi_rx_enable_group(ops,0,true); return 0; }
