#include "keystone_pcie_module_msi_reception_rc_mode.h"
int kpm_msi_rx_init(const pcie_core_ops_t* ops){ return ops->init(); }
int kpm_msi_rx_enable_group(const pcie_core_ops_t* ops, unsigned g, bool en){ return ops->msi_enable_group(g,en); }
