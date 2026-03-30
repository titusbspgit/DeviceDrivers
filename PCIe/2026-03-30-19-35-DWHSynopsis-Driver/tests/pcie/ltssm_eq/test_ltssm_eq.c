#include <stdio.h>
#include <string.h>
#define PCIE_HAL_HOST_TEST 1
#include "drivers/pcie/features/ltssm_eq/pcie_ltssm_eq.h"

static uint8_t dbi_mem[4096];

int test_ltssm_eq(void)
{
    pcie_dbi_t ctx; pcie_init(&ctx, dbi_mem, sizeof(dbi_mem)); memset(dbi_mem, 0, sizeof(dbi_mem));
    pcie_status_t rc = pcie_eq_set_local_fs_lf(&ctx, 20u, 10u);
    if ((rc!=PCIE_STATUS_OK) || (*(uint32_t*)&dbi_mem[DWC_GEN3_EQ_LOCAL_FS_LF_OFF] != ((20u<<6)|(10u<<0)))) { printf("fs/lf fail\n"); return -1; }

    rc = pcie_eq_program_preset_map(&ctx, 3u, 0x11223344u);
    if ((rc!=PCIE_STATUS_OK) || (*(uint32_t*)&dbi_mem[DWC_GEN3_EQ_PRESET_INDEX_OFF] != 3u) || (*(uint32_t*)&dbi_mem[DWC_GEN3_EQ_PSET_COEF_MAP_OFF] != 0x11223344u)) { printf("preset map fail\n"); return -1; }

    *(uint32_t*)&dbi_mem[DWC_GEN3_EQ_STATUS_OFF] = 0xDEADBEEFu; uint32_t eqst=0; rc = pcie_eq_get_status(&ctx, &eqst);
    if ((rc!=PCIE_STATUS_OK) || (eqst!=0xDEADBEEFu)) { printf("eq status fail\n"); return -1; }

    rc = pcie_port_send_vendor_dllp(&ctx, 0xAABBCCDDu);
    if ((rc!=PCIE_STATUS_OK) || (*(uint32_t*)&dbi_mem[DWC_PORT_VSDLLP_OFF] != 0xAABBCCDDu) || ((*(uint32_t*)&dbi_mem[DWC_PORT_FORCE_OFF] & 1u) == 0u)) { printf("dllp send fail\n"); return -1; }

    rc = pcie_port_force_ltssm_state(&ctx, 0x5u, true);
    if ((rc!=PCIE_STATUS_OK) || ((*(uint32_t*)&dbi_mem[DWC_PORT_FORCE_OFF] & (0xF0u|0x100u)) == 0u)) { printf("ltssm force fail\n"); return -1; }

    rc = pcie_port_force_ltssm_state(&ctx, 0u, false);
    if ((rc!=PCIE_STATUS_OK) || ((*(uint32_t*)&dbi_mem[DWC_PORT_FORCE_OFF] & (0xF0u|0x100u)) != 0u)) { printf("ltssm release fail\n"); return -1; }

    return 0;
}
