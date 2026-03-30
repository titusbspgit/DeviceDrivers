/* SPDX-License-Identifier: BSD-3-Clause */
/* Test for Synopsys DWC PCIe DM v6.00a — ASPM */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "../include/dw_pcie_aspm.h"

static volatile uint8_t mock_ecam[256u];
static volatile uint8_t mock_dbi[2048u];

int main(void)
{
    printf("[ASPM TEST] Start\n");
    memset((void *)mock_ecam, 0, sizeof(mock_ecam));
    memset((void *)mock_dbi, 0, sizeof(mock_dbi));

    const uint32_t pcie_cap_off = 0x40u;
    /* Advertise L0s+L1 */
    *(volatile uint32_t *)(mock_ecam + pcie_cap_off + DW_PCIE_CAP_LINK_CAP_OFF) = DW_PCIE_LCAP_ASPM_L0S_L1;

    dw_pcie_aspm_ctx_t ctx;
    if (dw_pcie_aspm_init(&ctx, (void *)mock_ecam, (void *)mock_dbi, pcie_cap_off) != 0) { printf("init fail\n"); return 1; }

    if ((dw_pcie_aspm_link_cap(&ctx) & DW_PCIE_LCAP_ASPM_L0S_L1) == 0u)
    { printf("cap fail\n"); return 1; }

    if (dw_pcie_aspm_set(&ctx, true, true) != 0) { printf("set fail\n"); return 1; }

    /* Verify bits toggled */
    uint32_t v = *(volatile uint32_t *)(mock_ecam + pcie_cap_off + DW_PCIE_CAP_LINK_CTRLSTS_OFF);
    if ((v & (DW_PCIE_LC_ASPM_L0S_EN | DW_PCIE_LC_ASPM_L1_EN)) != (DW_PCIE_LC_ASPM_L0S_EN | DW_PCIE_LC_ASPM_L1_EN))
    { printf("verify fail\n"); return 1; }

    /* Tune */
    dw_pcie_aspm_tune(&ctx, true, 3u);

    printf("[ASPM TEST] PASS\n");
    return 0;
}
