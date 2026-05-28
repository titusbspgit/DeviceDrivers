#include "keystone_pcie_module_dual_mode_operation_root_complex_or_end_point.h"

int kpm_dual_mode_init(const pcie_core_ops_t* ops)
{
    return ops->init();
}

int kpm_dual_mode_set_rc(const pcie_core_ops_t* ops)
{
    (void)ops; /* Role control is SoC-specific; placeholder */
    return 0;
}

int kpm_dual_mode_set_ep(const pcie_core_ops_t* ops)
{
    (void)ops; /* Role control is SoC-specific; placeholder */
    return 0;
}
