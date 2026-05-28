#include <stdint.h>
#include <stdio.h>
#include "core_keystone_pcie_module.h"
#include "keystone_pcie_module_link_width_and_speed_x1_x2_gen1_gen2.h"

static void test_link_params(void)
{
    KPM_CoreApi api = kpm_core_get_api();
    if (kpm_link_init(&api) != 0) {
        return; /* Unsupported */
    }
    (void)kpm_link_request(&api, KPM_WIDTH_X1, KPM_SPEED_GEN1);
    KPM_LinkWidth w; KPM_LinkSpeed s;
    (void)kpm_link_query(&api, &w, &s);
}

int main(void)
{
    test_link_params();
    return 0;
}
