/* SPDX-License-Identifier: BSD-3-Clause */
/* Test for Synopsys DWC PCIe DM v6.00a — SPCIE */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "../include/dw_pcie_spcie.h"
#include "../../feature_flit_mode/include/dw_pcie_flit.h" /* for dw_pcie_find_extcap decl */

static volatile uint8_t mock_ecam[512u];

static void write_extcap_hdr(uint32_t off, uint16_t id, uint32_t next)
{
    uint32_t hdr = 0u; hdr |= (uint32_t)id; hdr |= ((next & 0xFFFu) << 20);
    *(volatile uint32_t *)(mock_ecam + off) = hdr;
}

int main(void)
{
    printf("[SPCIE TEST] Start\n");
    memset((void *)mock_ecam, 0, sizeof(mock_ecam));

    write_extcap_hdr(0x100u, (uint16_t)DW_PCIE_SPCIE_EXT_CAP_ID, 0x000u);

    dw_pcie_spcie_ctx_t ctx;
    if (dw_pcie_spcie_init(&ctx, (void *)mock_ecam) != 0) { printf("init fail\n"); return 1; }
    (void)dw_pcie_spcie_discover(&ctx);
    if (!dw_pcie_spcie_present(&ctx)) { printf("not present\n"); return 1; }

    /* Test EQ request */
    if (dw_pcie_spcie_request_equalization(&ctx) != 0) { printf("eq req fail\n"); return 1; }

    /* Simulate lane errors and clear */
    *(volatile uint32_t *)(mock_ecam + 0x100u + DW_SPCIE_LANE_ERR_STS_OFF) = 0xFu;
    uint32_t st = dw_pcie_spcie_get_lane_err_status(&ctx);
    if (st != 0xFu) { printf("status bad 0x%08X\n", (unsigned)st); return 1; }
    if (dw_pcie_spcie_clear_lane_err_status(&ctx, 0x5u) != 0) { printf("clear fail\n"); return 1; }

    printf("[SPCIE TEST] PASS\n");
    return 0;
}
