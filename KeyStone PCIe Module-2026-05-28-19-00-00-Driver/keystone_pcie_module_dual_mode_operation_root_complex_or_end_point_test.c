#include "core_keystone_pcie_module.h"
#include "keystone_pcie_module_dual_mode_operation_root_complex_or_end_point.h"
int main(void){ const pcie_core_ops_t* ops = pcie_core_get_ops(); kpm_dual_mode_init(ops); return 0; }
