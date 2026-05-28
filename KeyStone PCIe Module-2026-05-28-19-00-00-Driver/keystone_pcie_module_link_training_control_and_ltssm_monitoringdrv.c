#include "keystone_pcie_module_link_training_control_and_ltssm_monitoring.h"
int kpm_ltssm_init(const pcie_core_ops_t* ops){ return ops->init(); }
int kpm_ltssm_enable(const pcie_core_ops_t* ops, bool en){ return ops->enable_ltssm(en); }
