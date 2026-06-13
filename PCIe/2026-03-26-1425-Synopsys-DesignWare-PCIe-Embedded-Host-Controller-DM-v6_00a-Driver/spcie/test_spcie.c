/* Test application for SPCIE */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "dw_pcie_dm_spcie.h"

#ifndef TEST_DBI_BASE
#define TEST_DBI_BASE (0u)
#endif

int main(void)
{
    dw_pcie_dm_spcie_ctx_t ctx;
    int32_t rc = dw_pcie_dm_spcie_init(&ctx, (uintptr_t)TEST_DBI_BASE);
    printf("[SPCIE] init rc=%d\n", (int)rc);
    bool present = false;
    rc = dw_pcie_dm_spcie_is_present(&ctx, &present);
    printf("[SPCIE] present=%d rc=%d\n", (int)present, (int)rc);
    if (!present) { return 0; }
    rc = dw_pcie_dm_spcie_set_eq_req_interrupt(&ctx, true);
    printf("[SPCIE] enable EQ-req interrupt rc=%d\n", (int)rc);
    rc = dw_pcie_dm_spcie_request_equalization(&ctx);
    printf("[SPCIE] request equalization rc=%d\n", (int)rc);
    uint32_t les = 0u; rc = dw_pcie_dm_spcie_read_lane_error_status(&ctx, &les);
    printf("[SPCIE] lane_err_status=0x%08x rc=%d\n", (unsigned)les, (int)rc);
    return 0;
}
