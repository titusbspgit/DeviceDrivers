/* Test for Link Training and EQ vendor controls */
#include <stdio.h>
#include <stdint.h>
#include "dw_pcie_dm_link_eq.h"

#ifndef TEST_DSP_BASE
#define TEST_DSP_BASE (0u)
#endif

int main(void)
{
    dw_pcie_dm_linkeq_ctx_t ctx;
    int32_t rc = dw_pcie_dm_linkeq_init(&ctx, (uintptr_t)TEST_DSP_BASE);
    printf("[LEQ] init rc=%d\n", (int)rc);
    if (rc != 0) { return 0; }
    /* RMW with zero mask should fail */
    rc = dw_pcie_dm_linkeq_rmw(&ctx, DWC_PORT_LINK_CTRL_OFF, 0u, 0u);
    printf("[LEQ] rmw zero-mask rc=%d (expect -1)\n", (int)rc);
    /* Program FS/LF (no-op if base is 0) */
    rc = dw_pcie_dm_linkeq_write_local_fs_lf(&ctx, 0x2Au, 0x1Bu);
    printf("[LEQ] write FS/LF rc=%d\n", (int)rc);
    /* Example: generic port force write with mask */
    rc = dw_pcie_dm_linkeq_port_force(&ctx, 0x0000000Fu, 0x00000005u);
    printf("[LEQ] port_force rmw rc=%d\n", (int)rc);
    return 0;
}
