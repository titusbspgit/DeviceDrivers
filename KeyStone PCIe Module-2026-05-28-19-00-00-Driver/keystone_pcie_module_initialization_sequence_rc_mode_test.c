#include "core_keystone_pcie_module.h"
#include "keystone_pcie_module_initialization_sequence_rc_mode.h"
int main(void){ const pcie_core_ops_t* ops=pcie_core_get_ops(); return kpm_rc_init_sequence(ops); }
