#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../hal/hal_mmio.h"
#include "../drivers/core/pcie_core.h"

int main(void)
{
    pcie_dev_t dev = { .base = (uintptr_t)PCIE_BASE_ADDR };

    printf("[CORE] init...\n");
    pcie_status_t st = pcie_core_init(&dev);
    if (st != PCIE_OK) {
        printf("[CORE] init failed: %d\n", (int)st);
        return -1;
    }

    uint32_t id = 0u;
    st = pcie_core_get_id(&dev, &id);
    printf("[CORE] ID=0x%08" PRIX32 " status=%d\n", id, (int)st);

    bool rdy = pcie_core_is_ready(&dev);
    printf("[CORE] ready=%d\n", rdy ? 1 : 0);

    st = pcie_core_soft_reset(&dev, 100000u);
    printf("[CORE] soft reset status=%d\n", (int)st);

    return 0;
}
