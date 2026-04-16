#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../hal/hal_mmio.h"
#include "../drivers/core/pcie_core.h"
#include "../drivers/link/pcie_link.h"

int main(void)
{
    pcie_dev_t dev = { .base = (uintptr_t)PCIE_BASE_ADDR };
    if (pcie_core_init(&dev) != PCIE_OK) {
        printf("[LINK] core init failed\n");
        return -1;
    }

    printf("[LINK] training...\n");
    pcie_status_t st = pcie_link_train(&dev, 500000u);
    printf("[LINK] train status=%d link_up=%d ltssm=0x%08X\n", (int)st, pcie_link_is_up(&dev) ? 1 : 0, pcie_link_ltssm_status(&dev));
    return 0;
}
