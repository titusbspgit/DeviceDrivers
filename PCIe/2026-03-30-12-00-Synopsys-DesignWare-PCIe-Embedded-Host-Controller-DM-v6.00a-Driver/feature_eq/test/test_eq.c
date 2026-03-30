/* SPDX-License-Identifier: BSD-3-Clause */
/* Test for Synopsys DWC PCIe DM v6.00a — Link Training & EQ Controls */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "../include/dw_pcie_eq.h"

static volatile uint8_t mock_dbi[4096u];

int main(void)
{
    printf("[EQ TEST] Start\n");
    memset((void *)mock_dbi, 0, sizeof(mock_dbi));

    dw_pcie_eq_ctx_t ctx;
    if (dw_pcie_eq_init(&ctx, (void *)mock_dbi) != 0) { printf("init fail\n"); return 1; }

    dw_pcie_eq_set_local_fs_lf(&ctx, 20u, 10u);
    uint8_t fs=0, lf=0; dw_pcie_eq_get_local_fs_lf(&ctx, &fs, &lf);
    if ((fs != 20u) || (lf != 10u)) { printf("fs/lf mismatch\n"); return 1; }

    if (dw_pcie_eq_set_preset(&ctx, 5u, 3u, 22u, 7u) != 0) { printf("set preset fail\n"); return 1; }
    uint8_t pre=0, cur=0, post=0;
    if (dw_pcie_eq_get_preset(&ctx, 5u, &pre, &cur, &post) != 0) { printf("get preset fail\n"); return 1; }

    /* Verify the value round-tripped in the single register mirror */
    if ((pre != 3u) || (cur != 22u) || (post != 7u)) { printf("preset mismatch\n"); return 1; }

    /* Simulate DLLP busy clear */
    *(volatile uint32_t *)(mock_dbi + DW_PCIE_VENDOR_SPEC_DLLP_OFF) = 0u; /* busy clear */
    if (dw_pcie_send_vendor_dllp(&ctx, 0x12345678u, 5u) != 0) { printf("dllp fail\n"); return 1; }

    /* Force retrain write */
    dw_pcie_force_retrain(&ctx);

    printf("[EQ TEST] PASS\n");
    return 0;
}
