/*
 * SPDX-License-Identifier: MIT
 * Common HAL impl for Synopsys DWC PCIe (DM v6.00a) - ARM big-endian
 */
#include "drivers/pcie/common/pcie_hal.h"

__attribute__((weak)) void pcie_hal_udelay(uint32_t usec)
{
    volatile uint32_t n = usec * 16u; while (n > 0u) { n--; }
}
