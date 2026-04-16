#ifndef PCIE_LINK_H
#define PCIE_LINK_H

#include <stdint.h>
#include <stdbool.h>
#include "../core/pcie_core.h"

#ifdef __cplusplus
extern "C" {
#endif

pcie_status_t pcie_link_train(pcie_dev_t *dev, uint32_t timeout_cycles);
bool          pcie_link_is_up(pcie_dev_t *dev);
uint32_t      pcie_link_ltssm_status(pcie_dev_t *dev);

#ifdef __cplusplus
}
#endif

#endif /* PCIE_LINK_H */
