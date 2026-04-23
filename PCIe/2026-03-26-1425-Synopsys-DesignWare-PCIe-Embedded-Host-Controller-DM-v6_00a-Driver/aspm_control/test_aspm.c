/* Test for ASPM control */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "dw_pcie_dm_aspm.h"

#ifndef TEST_DBI_BASE
#define TEST_DBI_BASE (0u)
#endif
#ifndef TEST_DSP_BASE
#define TEST_DSP_BASE (0u)
#endif

int main(void)
{
    dw_pcie_dm_aspm_ctx_t ctx;
    int32_t rc = dw_pcie_dm_aspm_init(&ctx, (uintptr_t)TEST_DBI_BASE, (uintptr_t)TEST_DSP_BASE);
    printf("[ASPM] init rc=%d\n", (int)rc);
    dw_pcie_aspm_mode_t mode = ASPM_DISABLE;
    rc = dw_pcie_dm_aspm_get_mode(&ctx, &mode);
    printf("[ASPM] get_mode rc=%d mode=%u\n", (int)rc, (unsigned)mode);
    rc = dw_pcie_dm_aspm_set_mode(&ctx, ASPM_L0S_L1);
    printf("[ASPM] set_mode(L0s+L1) rc=%d\n", (int)rc);
    rc = dw_pcie_dm_aspm_get_mode(&ctx, &mode);
    printf("[ASPM] get_mode rc=%d mode=%u\n", (int)rc, (unsigned)mode);

    /* Optional vendor DSP RMW (no-op if base is 0) */
    rc = dw_pcie_dm_aspm_vendor_rmw(&ctx, DWC_ACK_F_ASPM_CTRL_OFF, 0x0000000Fu, 0x00000003u);
    printf("[ASPM] vendor_rmw rc=%d\n", (int)rc);
    return 0;
}
