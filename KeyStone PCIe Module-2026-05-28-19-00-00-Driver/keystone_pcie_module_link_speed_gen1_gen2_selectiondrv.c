#include "keystone_pcie_module_link_speed_gen1_gen2_selection.h"
int kpm_link_speed_init(const pcie_core_ops_t* ops){ return ops->init(); }
int kpm_set_gen2(const pcie_core_ops_t* ops, bool enable){ return ops->set_link_speed_gen2(enable); }
