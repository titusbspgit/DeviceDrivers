#include <stdio.h>
#include <string.h>
#define PCIE_HAL_HOST_TEST 1
#include "drivers/pcie/features/flit_mode/pcie_flit_mode.h"

static uint8_t dbi_mem[4096];

static void init_mock_caps(void)
{
    memset(dbi_mem, 0, sizeof(dbi_mem));
    dbi_mem[PCI_CFG_CAP_PTR_OFF] = 0x50u;
    dbi_mem[0x50] = PCI_CAP_ID_PCI_EXP; /* ID */
    dbi_mem[0x51] = 0x00; /* Next */
    *(uint32_t*)&dbi_mem[0x50 + PCIE_CAP_REG_DEV_CAP3] = 0u | DEV_CAP3_FLIT_MODE_SUPP;
    *(uint32_t*)&dbi_mem[0x50 + PCIE_CAP_REG_LINK_CTRL3_STAT3] = 0u;
    *(uint32_t*)&dbi_mem[0x50 + PCIE_CAP_REG_LINK_CTRL2_STAT2] = 0u;
    *(uint32_t*)&dbi_mem[0x100] = (uint32_t)((PCIE_EXTCAP_ID_PL64G & 0xFFFFu) | (0x1u << 16) | (0x000u << 20));
    *(uint32_t*)&dbi_mem[0x10C] = 0xA5A50001u;
}

int test_flit(void)
{
    pcie_dbi_t ctx;
    pcie_init(&ctx, dbi_mem, sizeof(dbi_mem));
    init_mock_caps();

    pcie_flit_info_t info; bool active=false; uint32_t st=0; pcie_status_t rc;
    rc = pcie_flit_query(&ctx, &info);
    if ((rc!=PCIE_STATUS_OK) || !info.flit_supported) { printf("flit_query fail\n"); return -1; }

    rc = pcie_flit_enable(&ctx, true, 1000u);
    if (rc != PCIE_STATUS_TIMEOUT) {
        printf("flit_enable should timeout before we set active\n"); return -1;
    }
    /* Simulate link reports FLIT active */
    *(uint32_t*)&dbi_mem[0x50 + PCIE_CAP_REG_LINK_CTRL2_STAT2] |= LINK_STATUS2_FLIT_MODE_ACTIVE_MASK;
    rc = pcie_flit_enable(&ctx, true, 1000u);
    if (rc != PCIE_STATUS_OK) { printf("flit_enable 2 fail\n"); return -1; }

    rc = pcie_flit_status(&ctx, &active, &st);
    if ((rc!=PCIE_STATUS_OK) || (!active)) { printf("flit_status fail\n"); return -1; }

    /* Disable */
    *(uint32_t*)&dbi_mem[0x50 + PCIE_CAP_REG_LINK_CTRL2_STAT2] &= ~LINK_STATUS2_FLIT_MODE_ACTIVE_MASK;
    rc = pcie_flit_enable(&ctx, false, 1000u);
    if (rc!=PCIE_STATUS_OK) { printf("flit_disable fail\n"); return -1; }

    return 0;
}
