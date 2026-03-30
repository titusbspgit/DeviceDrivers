#include <stdio.h>
#include <string.h>
#define PCIE_HAL_HOST_TEST 1
#include "drivers/pcie/features/pipe_phy/pcie_pipe_phy.h"

static uint8_t dbi_mem[4096];

int test_pipe_phy(void)
{
    pcie_dbi_t ctx; pcie_status_t st = pcie_init(&ctx, dbi_mem, sizeof(dbi_mem)); if (st != PCIE_STATUS_OK) { printf("Init failed\n"); return -1; }

    memset(dbi_mem, 0, sizeof(dbi_mem)); const pcie_phy_bus_cfg_t cfg = PCIE_PHY_BUS_CFG_DEFAULT;

    *(uint32_t*)&dbi_mem[DWC_PHY_VIEWPORT_DATA_OFF] = 0x00001234u; *(uint32_t*)&dbi_mem[DWC_PHY_VIEWPORT_CTLSTS_OFF] = 0x0u;

    uint16_t out = 0u; st = pcie_phy_read(&ctx, &cfg, 0x0020u, &out, 0u, false, 1000u);
    if ((st!=PCIE_STATUS_OK) || (out!=0x1234u)) { printf("phy_read fail\n"); return -1; }

    *(uint32_t*)&dbi_mem[DWC_PHY_VIEWPORT_CTLSTS_OFF] = 0x0u;
    st = pcie_phy_write(&ctx, &cfg, 0x0030u, 0xBEEF, 0u, false, 1000u);
    if ((st!=PCIE_STATUS_OK) || (*(uint32_t*)&dbi_mem[DWC_PHY_VIEWPORT_DATA_OFF] != 0x0000BEEFu)) { printf("phy_write fail\n"); return -1; }
    return 0;
}
