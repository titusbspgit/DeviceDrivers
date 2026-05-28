#ifndef KEYSTONE_PCIE_MODULE_MSI_RECEPTION_RC_MODE_H
#define KEYSTONE_PCIE_MODULE_MSI_RECEPTION_RC_MODE_H
#include "core_keystone_pcie_module.h"
int kpm_msi_rx_init(const pcie_core_ops_t* ops);
int kpm_msi_rx_enable_group(const pcie_core_ops_t* ops, unsigned g, bool en);
#endif
