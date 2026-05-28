#include "core_keystone_pcie_module.h"
#include "keystone_pcie_module_link_width_x1_x2_with_lane_reversal_and_polarity_inversion.h"
int main(void){ const pcie_core_ops_t* ops=pcie_core_get_ops(); return kpm_link_width_init(ops); }
