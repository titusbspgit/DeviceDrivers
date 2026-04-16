#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../hal/hal_mmio.h"
#include "../drivers/core/pcie_core.h"
#include "../drivers/dma/pcie_dma.h"

int main(void)
{
    pcie_dev_t dev = { .base = (uintptr_t)PCIE_BASE_ADDR };
    if (pcie_core_init(&dev) != PCIE_OK) {
        printf("[DMA] core init failed\n");
        return -1;
    }

    /* This is a stub test; addresses are placeholders. */
    (void)pcie_dma_start_memcpy(&dev, 0x80000000u, 0x81000000u, 256u);
    printf("[DMA] started memcpy (stub) busy=%d\n", pcie_dma_is_busy(&dev) ? 1 : 0);
    return 0;
}
