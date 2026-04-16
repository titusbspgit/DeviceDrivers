#ifndef PCIE_DMA_H
#define PCIE_DMA_H

#include <stdint.h>
#include <stdbool.h>
#include "../core/pcie_core.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Minimal DMA API (spec-dependent). Implemented as stub with TODOs. */
pcie_status_t pcie_dma_start_memcpy(pcie_dev_t *dev, uint32_t src, uint32_t dst, uint32_t len_bytes);
bool          pcie_dma_is_busy(pcie_dev_t *dev);

#ifdef __cplusplus
}
#endif

#endif /* PCIE_DMA_H */
