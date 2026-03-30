/* SPDX-License-Identifier: BSD-3-Clause */
/* Test for Synopsys DWC PCIe DM v6.00a — PIPE PHY Interface */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "../include/dw_pcie_pipe.h"

static volatile uint8_t mock_dbi[4096u];

int main(void)
{
    printf("[PIPE TEST] Start\n");
    memset((void *)mock_dbi, 0, sizeof(mock_dbi));

    /* Simulate PHY ready */
    /* Set READY bit */
    const uint32_t ready = DW_PHY_STATUS_READY;
    const volatile uint32_t *p = (const volatile uint32_t *)(mock_dbi + DW_PCIE_PHY_STATUS_OFF);
    *(volatile uint32_t *)p = ready;

    dw_pcie_pipe_ctx_t ctx;
    if (dw_pcie_pipe_init(&ctx, (void *)mock_dbi) != 0)
    { printf("[PIPE TEST] init failed\n"); return 1; }

    if (!dw_pcie_phy_ready(&ctx))
    { printf("[PIPE TEST] not ready\n"); return 1; }

    /* Emulate viewport: clear BUSY quickly when START is written */
    /* For testing, we will pre-clear BUSY and place data in DATA reg for reads */
    *(volatile uint32_t *)(mock_dbi + DW_PCIE_PHY_VIEWPORT_CTLSTS_OFF) = 0u;
    *(volatile uint32_t *)(mock_dbi + DW_PCIE_PHY_VIEWPORT_DATA_OFF) = 0xA5A5A5A5u;

    uint32_t data = 0u;
    if (dw_pcie_phy_read(&ctx, 0x10u, &data, 5u) != 0)
    { printf("[PIPE TEST] read failed\n"); return 1; }
    if (data != 0xA5A5A5A5u)
    { printf("[PIPE TEST] read mismatch 0x%08X\n", (unsigned)data); return 1; }

    if (dw_pcie_phy_write(&ctx, 0x11u, 0x55AA55AAu, 5u) != 0)
    { printf("[PIPE TEST] write failed\n"); return 1; }

    /* Verify DATA register latched */
    uint32_t dlat = *(volatile uint32_t *)(mock_dbi + DW_PCIE_PHY_VIEWPORT_DATA_OFF);
    if (dlat != 0x55AA55AAu)
    { printf("[PIPE TEST] write data not latched 0x%08X\n", (unsigned)dlat); return 1; }

    /* Configure message bus depths */
    dw_pcie_pipe_config_msg_bus(&ctx, 3u, 4u);

    printf("[PIPE TEST] PASS\n");
    return 0;
}
