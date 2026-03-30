#include <stdio.h>
#include <string.h>
#define PCIE_HAL_HOST_TEST 1
#include "drivers/pcie/features/spcie/pcie_spcie.h"

static uint8_t dbi_mem[1024];

static void init_spcie(void)
{
    memset(dbi_mem, 0, sizeof(dbi_mem));
    *(uint32_t*)&dbi_mem[0x100] = (uint32_t)((PCIE_EXTCAP_ID_SPCIE & 0xFFFFu) | (0x1u << 16) | (0x000u << 20));
    *(uint32_t*)&dbi_mem[0x104] = 0xABCD0001u; /* Link Control3=0x0001, Status3=0xABCD */
    *(uint32_t*)&dbi_mem[0x108] = 0x00000005u; /* Lane err */
}

int test_spcie(void)
{
    pcie_dbi_t ctx; pcie_init(&ctx, dbi_mem, sizeof(dbi_mem)); init_spcie();
    pcie_spcie_link3_t l3; pcie_status_t rc;
    rc = pcie_spcie_get_link3(&ctx, &l3);
    if ((rc!=PCIE_STATUS_OK) || (l3.link_control3!=0x0001u) || (l3.link_status3!=0xABCDu)) { printf("get_link3 fail\n"); return -1; }

    rc = pcie_spcie_set_link_control3(&ctx, 0x00F0u);
    if ((rc!=PCIE_STATUS_OK) || (*(uint32_t*)&dbi_mem[0x104] != 0xABCD00F0u)) { printf("set_link3 fail\n"); return -1; }

    uint32_t mask=0; rc = pcie_spcie_get_lane_err_status(&ctx, &mask);
    if ((rc!=PCIE_STATUS_OK) || (mask!=0x5u)) { printf("lane_err rd fail\n"); return -1; }

    rc = pcie_spcie_clear_lane_err_status(&ctx, 0x00000005u);
    if ((rc!=PCIE_STATUS_OK)) { printf("lane_err clr st fail\n"); return -1; }

    return 0;
}
