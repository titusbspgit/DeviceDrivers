/*
 * Test - Gen1/Gen2 link speed selection (2.5/5.0 GT/s)
 * Author: ag-drv-CodeGen Agent
 * Date: 2026-05-29
 */
#include <stdint.h>
#include <stdbool.h>
#include "core_keystone_pcie_module.h"
#include "keystone_pcie_module_gen1_gen2_link_speed_selection_2_5_5_0_gt_s.h"

static void platform_init(void)
{
    /* Stub for clock/reset domain init; not implemented. */
}

int main(void)
{
    platform_init();

    KPM_Core core;
    kpm_core_init(&core, KPM_BASE_ADDR);
    const KPM_CoreAPI *api = kpm_core_get_api();
    (void)api; /* Reserved for future register accesses */

    kpm_linkspeed_ctx_t ctx = {0};
    if (kpm_linkspeed_init(&ctx, &core) != KPM_STATUS_OK) {
        return -1;
    }

    /* Attempt to set GEN2 speed (expected UNSUPPORTED until mapping is known). */
    if (kpm_linkspeed_set(&ctx, KPM_LINK_SPEED_GEN2) != KPM_STATUS_UNSUPPORTED) {
        return -2;
    }

    /* Query speed (expected UNSUPPORTED). */
    kpm_link_speed_t cur;
    if (kpm_linkspeed_get(&ctx, &cur) != KPM_STATUS_UNSUPPORTED) {
        return -3;
    }

    /* Toggle training (expected UNSUPPORTED). */
    if (kpm_linkspeed_training_enable(&ctx, true) != KPM_STATUS_UNSUPPORTED) {
        return -4;
    }

    return 0;
}
