#include <stdint.h>
#include <stdio.h>
#include "core_driver.h"
#include "keystone_pcie_module_gen1_gen2_link_speed_selection.h"

int main(void)
{
    const struct pcie_core_api *api = pcie_core_get_api();

    /* Request Gen2 then retrain */
    if (kpm_set_target_link_speed(api, KPM_LINK_GEN2) == 0) {
        if (kpm_retrain_link_and_wait(api, 1000000) == 0) {
            uint32_t sp = kpm_get_negotiated_speed(api);
            (void)sp; /* verify sp == 2 for Gen2 */
        }
    }
    return 0;
}
