#include <stdio.h>
#include <string.h>
#define PCIE_HAL_HOST_TEST 1
#include "drivers/pcie/features/pipe_phy/pcie_pipe_phy.h"

static uint8_t dbi_mem[4096];

int test_pipe_phy(void)
{
    pcie_dbi_t ctx; pcie_init(&ctx, dbi_mem, sizeof(dbi_mem)); memset(dbi_mem, 0, sizeof(dbi_mem));
    const pcie_phy_bus_cfg_t cfg = PCIE_PHY_BUS_CFG_DEFAULT;
    *(uint32_t*)&dbi_mem[DWC_PHY_VIEWPORT_CTLSTS_OFF] = 0x0u; /* BUSY==0 */
    *(uint32_t*)&dbi_mem[DWC_PHY_VIEWPORT_DATA_OFF]   = 0x00001234u;

    uint16_t out=0; pcie_status_t rc;
    rc = pcie_phy_read(&ctx, &cfg, 0x20u, &out, 0u, false, 1000u);
    if ((rc!=PCIE_STATUS_OK) || (out!=0x1234u)) { printf("phy_read fail\n"); return -1; }

    *(uint32_t*)&dbi_mem[DWC_PHY_VIEWPORT_CTLSTS_OFF] = 0x0u; /* ensure not busy */
    rc = pcie_phy_write(&ctx, &cfg, 0x30u, 0xBEEFu, 0u, false, 1000u);
    if ((rc!=PCIE_STATUS_OK) || (*(uint32_t*)&dbi_mem[DWC_PHY_VIEWPORT_DATA_OFF] != 0x0000BEEFu))
    { printf("phy_write fail\n"); return -1; }

    return 0;
}
