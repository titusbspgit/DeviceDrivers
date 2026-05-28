#include <stdint.h>
#include "keystone_pcie_module_link_width_and_speed_x1_x2_gen1_gen2.h"

__attribute__((weak)) void kpm_test_log(const char *msg) { (void)msg; }

int keystone_pcie_module_link_width_and_speed_x1_x2_gen1_gen2_test(void)
{
    const kpm_core_api_t *api = kpm_core_get_default();
    kpm_status_t st;
    kpm_pcie_link_cfg_t cfg = { .width = KPM_PCIE_LINK_WIDTH_X1, .gen = KPM_PCIE_GEN1 };

    st = kpm_pcie_link_configure(api, &cfg);
    if (st != KPM_STATUS_NOT_IMPLEMENTED) {
        kpm_test_log("link_configure unexpected status\n");
        return 1;
    }

    kpm_pcie_link_cfg_t s = {0};
    st = kpm_pcie_link_get_status(api, &s);
    if (st != KPM_STATUS_NOT_IMPLEMENTED) {
        kpm_test_log("link_get_status unexpected status\n");
        return 2;
    }

    return 0;
}
