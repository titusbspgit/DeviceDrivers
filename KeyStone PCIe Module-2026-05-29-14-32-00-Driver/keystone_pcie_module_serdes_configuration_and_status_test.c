/*
 * Test - SerDes configuration and status
 * Author: ag-drv-CodeGen Agent
 * Date: 2026-05-29
 */
#include <stdint.h>
#include <stdbool.h>
#include "core_keystone_pcie_module.h"
#include "keystone_pcie_module_serdes_configuration_and_status.h"

static void platform_init(void)
{
    /* Stub for clock/reset handling; not implemented. */
}

int main(void)
{
    platform_init();

    KPM_Core core;
    kpm_core_init(&core, KPM_BASE_ADDR);

    kpm_serdes_ctx_t ctx = {0};
    if (kpm_serdes_init(&ctx, &core) != KPM_STATUS_OK) {
        return -1;
    }

    kpm_serdes_pll_cfg_t pll = { .pll_mpy = 0u, .pll_cfg = 0u }; /* UNKNOWN values */

    if (kpm_serdes_configure_pll(&ctx, &pll) != KPM_STATUS_UNSUPPORTED) {
        return -2;
    }

    if (kpm_serdes_wait_lock(&ctx, 100000u) != KPM_STATUS_UNSUPPORTED) {
        return -3;
    }

    if (kpm_serdes_enable_lane(&ctx, 0u, true) != KPM_STATUS_UNSUPPORTED) {
        return -4;
    }

    return 0;
}
