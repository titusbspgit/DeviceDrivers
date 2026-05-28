#include "keystone_pcie_module_msi_generation_ep_mode.h"
int kpm_msi_tx_init(const pcie_core_ops_t* ops){ return ops->init(); }
int kpm_msi_tx_fire(const pcie_core_ops_t* ops, unsigned vector){ (void)vector; return ops->msi_ack(0); }
