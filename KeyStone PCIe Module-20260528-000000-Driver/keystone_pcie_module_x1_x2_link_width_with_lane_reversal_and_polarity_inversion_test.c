#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "core_driver.h"
#include "keystone_pcie_module_x1_x2_link_width_with_lane_reversal_and_polarity_inversion.h"

int main(void)
{
    const struct pcie_core_api *api = pcie_core_get_api();

    /* Try to prevent downtraining */
    (void)kpm_set_auto_width_downtrain_disable(api, true);
    /* Read negotiated width */
    uint32_t w = kpm_get_negotiated_width(api);
    (void)w;
    return 0;
}
