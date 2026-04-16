#ifndef PCIE_MSI_H
#define PCIE_MSI_H

#include <stdint.h>
#include <stdbool.h>
#include "../core/pcie_core.h"

#ifdef __cplusplus
extern "C" {
#endif

pcie_status_t pcie_msi_enable(pcie_dev_t *dev, bool enable);
pcie_status_t pcie_msi_program(pcie_dev_t *dev, uint32_t addr, uint16_t data);

#ifdef __cplusplus
}
#endif

#endif /* PCIE_MSI_H */
