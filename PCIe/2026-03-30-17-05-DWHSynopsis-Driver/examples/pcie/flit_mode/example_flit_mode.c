#include <stdio.h>
#include <string.h>
#include "drivers/pcie/features/flit_mode/pcie_flit_mode.h"

int main(void)
{
    /* Example stub: platform must map real DBI base */
    volatile uint8_t *dbi = (volatile uint8_t*)0xFFFFFFFFu; /* placeholder */
    pcie_dbi_t ctx; if (pcie_init(&ctx, (void*)dbi, 0x2000u) != PCIE_STATUS_OK) { return -1; }
    pcie_flit_info_t info; if (pcie_flit_query(&ctx, &info)!=PCIE_STATUS_OK) { return -1; }
    if (info.flit_supported) { (void)pcie_flit_enable(&ctx, true, 50000u); }
    return 0;
}
