#ifndef KEYSTONE_PCIE_MODULE_LINK_SPEED_GEN1_GEN2_SELECTION_H
#define KEYSTONE_PCIE_MODULE_LINK_SPEED_GEN1_GEN2_SELECTION_H
#include "core_keystone_pcie_module.h"
int kpm_link_speed_init(const pcie_core_ops_t* ops);
int kpm_set_gen2(const pcie_core_ops_t* ops, bool enable);
#endif
