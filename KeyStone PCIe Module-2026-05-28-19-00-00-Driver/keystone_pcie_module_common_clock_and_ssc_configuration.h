#ifndef KEYSTONE_PCIE_MODULE_COMMON_CLOCK_AND_SSC_CONFIGURATION_H
#define KEYSTONE_PCIE_MODULE_COMMON_CLOCK_AND_SSC_CONFIGURATION_H
#include "core_keystone_pcie_module.h"
int kpm_common_clock_init(const pcie_core_ops_t* ops);
int kpm_common_clock_set(const pcie_core_ops_t* ops, bool en);
#endif
