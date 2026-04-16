#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../hal/hal_mmio.h"
#include "../drivers/core/pcie_core.h"
#include "../drivers/cfg/pcie_cfg.h"

int main(void)
{
    pcie_dev_t dev = { .base = (uintptr_t)PCIE_BASE_ADDR };
    if (pcie_core_init(&dev) != PCIE_OK) {
        printf("[CFG] core init failed\n");
        return -1;
    }

    uint32_t v = 0u;
    if (pcie_cfg_read(&dev, 0u, 0u, 0u, 0x00u, PCIE_CFG_WIDTH_32, &v) == PCIE_OK) {
        printf("[CFG] Dev/Vendor ID DWORD0 = 0x%08X\n", v);
    } else {
        printf("[CFG] read failed\n");
    }

    /* Edge case: unaligned 8-bit write within DWORD */
    (void)pcie_cfg_write(&dev, 0u, 0u, 0u, 0x0Du, PCIE_CFG_WIDTH_8, 0xABu);

    return 0;
}
