#ifndef PCIE_CORE_H
#define PCIE_CORE_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PCIE_OK = 0,
    PCIE_EINVAL = -1,
    PCIE_EBUSY = -2,
    PCIE_ETIMEOUT = -3
} pcie_status_t;

typedef struct {
    uintptr_t base;
} pcie_dev_t;

pcie_status_t pcie_core_init(pcie_dev_t *dev);
pcie_status_t pcie_core_soft_reset(pcie_dev_t *dev, uint32_t timeout_cycles);
pcie_status_t pcie_core_get_id(pcie_dev_t *dev, uint32_t *out_id);
bool          pcie_core_is_ready(pcie_dev_t *dev);

#ifdef __cplusplus
}
#endif

#endif /* PCIE_CORE_H */
