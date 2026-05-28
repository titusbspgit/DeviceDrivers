#include "core_keystone_pcie_module.h"
#include "keystone_pcie_module_common_clock_and_ssc_configuration.h"
int main(void){ const pcie_core_ops_t* ops=pcie_core_get_ops(); kpm_common_clock_init(ops); kpm_common_clock_set(ops,true); return 0; }
