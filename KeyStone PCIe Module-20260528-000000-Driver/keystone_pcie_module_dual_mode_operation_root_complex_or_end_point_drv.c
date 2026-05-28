/* Driver: Dual-mode operation (Root Complex or End Point) */
#include "keystone_pcie_module_dual_mode_operation_root_complex_or_end_point.h"

/* No registers available for mode override; only helper checks. */
int kpm_dual_mode_init(const struct pcie_core_api *api)
{
    (void)api;
    /* Documented: select mode via bootstrap; ensure LTSSM disabled during selection. */
    return 0;
}
