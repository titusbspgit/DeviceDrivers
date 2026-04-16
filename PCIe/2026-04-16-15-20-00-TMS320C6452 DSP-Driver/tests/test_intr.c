#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../hal/hal_mmio.h"
#include "../drivers/core/pcie_core.h"
#include "../drivers/intr/pcie_intr.h"
#include "../include/pcie_regs.h"

int main(void)
{
    pcie_dev_t dev = { .base = (uintptr_t)PCIE_BASE_ADDR };
    if (pcie_core_init(&dev) != PCIE_OK) {
        printf("[INTR] core init failed\n");
        return -1;
    }

    (void)pcie_intr_enable_mask(&dev, PCIE_INT_LINK | PCIE_INT_MSI | PCIE_INT_DMA);
    uint32_t m = pcie_intr_status(&dev);
    printf("[INTR] status before clear: 0x%08X\n", m);

    /* Simulate: write status bits (W1C) */
    (void)pcie_intr_clear(&dev, PCIE_INT_LINK | PCIE_INT_MSI);
    printf("[INTR] cleared link+msi\n");

    (void)pcie_intr_disable_mask(&dev, PCIE_INT_DMA);
    printf("[INTR] masked DMA\n");

    return 0;
}
