#include <stdint.h>
#include <stdio.h>
#include "core_driver.h"
#include "keystone_pcie_module_dual_mode_operation_root_complex_or_end_point.h"

int main(void)
{
    const struct pcie_core_api *api = pcie_core_get_api();
    (void)api;

    enum kpm_mode m = kpm_get_mode(api);
    if (m == KPM_MODE_UNKNOWN) {
        /* Missing data -> UNKNOWN; just report */
        /* On real platform, mode can be inferred from config header type. */
    }

    /* Ensure LTSSM remains disabled while mode is considered fixed */
    (void)kpm_mode_finalize_before_ltssm(api);
    return 0;
}
