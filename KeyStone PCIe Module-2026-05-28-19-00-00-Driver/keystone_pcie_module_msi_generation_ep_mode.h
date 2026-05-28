#ifndef KEYSTONE_PCIE_MODULE_MSI_GENERATION_EP_MODE_H
#define KEYSTONE_PCIE_MODULE_MSI_GENERATION_EP_MODE_H
#include "core_keystone_pcie_module.h"
int kpm_msi_tx_init(const pcie_core_ops_t* ops);
int kpm_msi_tx_fire(const pcie_core_ops_t* ops, unsigned vector);
#endif
