#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../hal/hal_mmio.h"
#include "../drivers/core/pcie_core.h"
#include "../drivers/msi/pcie_msi.h"

int main(void)
{
    pcie_dev_t dev = { .base = (uintptr_t)PCIE_BASE_ADDR };
    if (pcie_core_init(&dev) != PCIE_OK) {
        printf("[MSI] core init failed\n");
        return -1;
    }

    (void)pcie_msi_enable(&dev, true);
    (void)pcie_msi_program(&dev, 0xFFFF0000u, 0x55AAu);
    printf("[MSI] enabled and programmed (address/data set)\n");
    return 0;
}
