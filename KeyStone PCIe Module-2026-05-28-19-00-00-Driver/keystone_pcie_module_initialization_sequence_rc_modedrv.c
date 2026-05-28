#include "keystone_pcie_module_initialization_sequence_rc_mode.h"
int kpm_rc_init_sequence(const pcie_core_ops_t* ops)
{
    int rc = ops->init(); if (rc) return rc;
    ops->endianness_set(true);
    ops->enable_ltssm(false);
    ops->set_common_clock(true);
    ops->set_link_speed_gen2(false);
    ops->enable_ltssm(true);
    return 0;
}
