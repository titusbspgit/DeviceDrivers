/*
 * Test - MSI vector enable/disable granularity
 * Author: ag-drv-CodeGen Agent
 * Date: 2026-05-29
 */
#include <stdint.h>
#include <stdbool.h>
#include "core_keystone_pcie_module.h"
#include "keystone_pcie_module_msi_vector_enable_disable_granularity.h"

static void platform_init(void)
{
    /* Stub for clock/reset/IRQ controller init; not implemented. */
}

int main(void)
{
    platform_init();

    KPM_Core core;
    kpm_core_init(&core, KPM_BASE_ADDR);

    kpm_msi_ctx_t ctx = {0};
    if (kpm_msi_init(&ctx, &core) != KPM_STATUS_OK) {
        return -1;
    }

    /* Try enabling vector 0 (expected UNSUPPORTED until mapping is known). */
    if (kpm_msi_enable_vector(&ctx, 0u) != KPM_STATUS_UNSUPPORTED) {
        return -2;
    }

    /* Check pending status (expected UNSUPPORTED). */
    bool pending = false;
    if (kpm_msi_is_pending(&ctx, 0u, &pending) != KPM_STATUS_UNSUPPORTED) {
        return -3;
    }

    /* Acknowledge (expected UNSUPPORTED). */
    if (kpm_msi_ack(&ctx, 0u) != KPM_STATUS_UNSUPPORTED) {
        return -4;
    }

    /* Disable (expected UNSUPPORTED). */
    if (kpm_msi_disable_vector(&ctx, 0u) != KPM_STATUS_UNSUPPORTED) {
        return -5;
    }

    return 0;
}
