#include <stdint.h>
#include "keystone_pcie_module_dual-mode_operation_rc_ep.h"

__attribute__((weak)) void kpm_test_log(const char *msg) { (void)msg; }

int keystone_pcie_module_dual_mode_operation_rc_ep_test(void)
{
    const kpm_core_api_t *api = kpm_core_get_default();
    kpm_status_t st;

    st = kpm_pcie_set_mode(api, KPM_PCIE_MODE_RC);
    if (st != KPM_STATUS_NOT_IMPLEMENTED) {
        kpm_test_log("set_mode unexpected status\n");
        return 1;
    }

    kpm_pcie_mode_t mode = KPM_PCIE_MODE_UNKNOWN;
    st = kpm_pcie_get_mode(api, &mode);
    if (st != KPM_STATUS_NOT_IMPLEMENTED || mode != KPM_PCIE_MODE_UNKNOWN) {
        kpm_test_log("get_mode unexpected result\n");
        return 2;
    }

    st = kpm_pcie_apply_mode(api);
    if (st != KPM_STATUS_NOT_IMPLEMENTED) {
        kpm_test_log("apply_mode unexpected status\n");
        return 3;
    }

    return 0;
}
