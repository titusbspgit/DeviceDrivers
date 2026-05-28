#ifndef KEYSTONE_PCIE_MODULE_DUAL_MODE_OPERATION_ROOT_COMPLEX_OR_END_POINT_H
#define KEYSTONE_PCIE_MODULE_DUAL_MODE_OPERATION_ROOT_COMPLEX_OR_END_POINT_H
#include "core_keystone_pcie_module.h"
#ifdef __cplusplus
extern "C" {
#endif
int kpm_dual_mode_init(const pcie_core_ops_t* ops);
int kpm_dual_mode_set_rc(const pcie_core_ops_t* ops);
int kpm_dual_mode_set_ep(const pcie_core_ops_t* ops);
#ifdef __cplusplus
}
#endif
#endif
