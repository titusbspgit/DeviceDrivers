#ifndef PCIE_INTR_H
#define PCIE_INTR_H

#include <stdint.h>
#include <stdbool.h>
#include "../core/pcie_core.h"

#ifdef __cplusplus
extern "C" {
#endif

pcie_status_t pcie_intr_enable_mask(pcie_dev_t *dev, uint32_t mask);
pcie_status_t pcie_intr_disable_mask(pcie_dev_t *dev, uint32_t mask);
uint32_t      pcie_intr_status(pcie_dev_t *dev);
pcie_status_t pcie_intr_clear(pcie_dev_t *dev, uint32_t mask);

#ifdef __cplusplus
}
#endif

#endif /* PCIE_INTR_H */
