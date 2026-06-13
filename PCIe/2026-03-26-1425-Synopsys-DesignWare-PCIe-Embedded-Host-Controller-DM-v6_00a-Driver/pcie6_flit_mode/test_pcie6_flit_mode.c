/* Test application for PL64G (PCIe 6.0 64G FLIT Mode) */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "dw_pcie_dm_pcie6_flit_mode.h"

#ifndef TEST_DBI_BASE
#define TEST_DBI_BASE (0u) /* Provide real DBI base via compile-time define */
#endif

static void test_pl64g_sequence(uintptr_t dbi)
{
    dw_pcie_dm_pl64g_ctx_t ctx;
    int32_t rc = dw_pcie_dm_pl64g_init(&ctx, dbi);
    if (rc != DW_PCIE_DM_OK) {
        printf("[PL64G] init failed: %d\n", (int)rc);
        return;
    }
    bool present = false;
    rc = dw_pcie_dm_pl64g_is_present(&ctx, &present);
    if (rc != DW_PCIE_DM_OK) {
        printf("[PL64G] presence check err: %d\n", (int)rc);
        return;
    }
    printf("[PL64G] present: %s\n", present ? "yes" : "no");
    if (!present) {
        return;
    }
    uint32_t status = 0u;
    rc = dw_pcie_dm_pl64g_read_status(&ctx, &status);
    printf("[PL64G] read status rc=%d val=0x%08x\n", (int)rc, (unsigned)status);

    bool done = false;
    rc = dw_pcie_dm_pl64g_wait_eq_complete(&ctx, 100000u, &done);
    printf("[PL64G] wait EQ rc=%d done=%d\n", (int)rc, (int)done);

    bool req = false, on = false;
    rc = dw_pcie_dm_pl64g_precoding_status(&ctx, &req, &on);
    printf("[PL64G] precoding: req=%d on=%d (rc=%d)\n", (int)req, (int)on, (int)rc);
}

int main(void)
{
    printf("DW PCIe DM v6.00a - PL64G test start\n");
    test_pl64g_sequence((uintptr_t)TEST_DBI_BASE);
    printf("DW PCIe DM v6.00a - PL64G test done\n");
    return 0;
}
