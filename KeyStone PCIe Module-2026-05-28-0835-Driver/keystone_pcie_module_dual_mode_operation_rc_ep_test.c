#include <stdint.h>
#include <stdio.h>
#include "core_keystone_pcie_module.h"
#include "keystone_pcie_module_dual_mode_operation_rc_ep.h"

static void test_dual_mode(void)
{
    KPM_CoreApi api = kpm_core_get_api();
    if (kpm_dual_mode_init(&api) != 0) {
        /* Unsupported or invalid configuration */
        return;
    }
    (void)kpm_dual_mode_set_role(&api, KPM_ROLE_RC);
    KPM_DualModeRole role = KPM_ROLE_EP;
    (void)kpm_dual_mode_get_role(&api, &role);
    (void)kpm_dual_mode_set_role(&api, KPM_ROLE_EP);
}

int main(void)
{
    test_dual_mode();
    return 0;
}
