#include <stdio.h>
#include <string.h>
#define PCIE_HAL_HOST_TEST 1
#include "drivers/pcie/features/aspm/pcie_aspm.h"

static uint8_t dbi_mem[2048];

static void init_pcie_cap(void)
{
    memset(dbi_mem, 0, sizeof(dbi_mem));
    dbi_mem[PCI_CFG_CAP_PTR_OFF] = 0x60u; dbi_mem[0x60] = PCI_CAP_ID_PCI_EXP; dbi_mem[0x61] = 0x00;
    *(uint16_t*)&dbi_mem[0x60 + 0x10] = 0x0000u;
}

int test_aspm(void)
{
    pcie_dbi_t ctx; pcie_init(&ctx, dbi_mem, sizeof(dbi_mem)); init_pcie_cap();

    pcie_status_t st = pcie_aspm_set_policy(&ctx, PCIE_ASPM_L1);
    if ((st!=PCIE_STATUS_OK) || (*(uint16_t*)&dbi_mem[0x60 + 0x10] != (uint16_t)PCIE_ASPM_L1)) { printf("set_policy fail\n"); return -1; }

    pcie_aspm_policy_t pol = PCIE_ASPM_DISABLED; st = pcie_aspm_get_policy(&ctx, &pol);
    if ((st!=PCIE_STATUS_OK) || (pol!=PCIE_ASPM_L1)) { printf("get_policy fail\n"); return -1; }

    st = pcie_aspm_tune_ack_params(&ctx, 0x10, 0x20, 0x30, 2, 4, true);
    if ((st!=PCIE_STATUS_OK) || (*(uint32_t*)&dbi_mem[0x70C] == 0u)) { printf("tune_ack fail\n"); return -1; }
    return 0;
}
