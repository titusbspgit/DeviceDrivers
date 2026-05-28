#include "keystone_pcie_module_common_clock_and_ssc_configuration.h"
int kpm_common_clock_init(const pcie_core_ops_t* ops){ return ops->init(); }
int kpm_common_clock_set(const pcie_core_ops_t* ops, bool en){ return ops->set_common_clock(en); }
