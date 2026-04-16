// SPDX-License-Identifier: MIT
#include "reg_access.h"

#ifdef RCI_MMIO_MOCK
#include <string.h>
uint8_t *rci_mmio_mock_buf = (uint8_t *)0;
uintptr_t rci_mmio_mock_base = (uintptr_t)0;
size_t rci_mmio_mock_size = 0u;

void rci_mock_init(uintptr_t base, uint8_t *buffer, size_t size_bytes)
{
    rci_mmio_mock_base = base;
    rci_mmio_mock_buf = buffer;
    rci_mmio_mock_size = size_bytes;
    if ((rci_mmio_mock_buf != (uint8_t *)0) && (rci_mmio_mock_size > 0u)) {
        (void)memset(rci_mmio_mock_buf, 0, rci_mmio_mock_size);
    }
}
#endif
