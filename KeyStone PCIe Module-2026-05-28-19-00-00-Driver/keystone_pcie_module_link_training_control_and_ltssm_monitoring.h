#ifndef KEYSTONE_PCIE_MODULE_LINK_TRAINING_CONTROL_AND_LTSSM_MONITORING_H
#define KEYSTONE_PCIE_MODULE_LINK_TRAINING_CONTROL_AND_LTSSM_MONITORING_H
#include "core_keystone_pcie_module.h"
int kpm_ltssm_init(const pcie_core_ops_t* ops);
int kpm_ltssm_enable(const pcie_core_ops_t* ops, bool en);
#endif
