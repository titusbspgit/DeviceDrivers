#ifndef PCIE_CFG_H
#define PCIE_CFG_H

#include <stdint.h>
#include <stdbool.h>
#include "../core/pcie_core.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PCIE_CFG_WIDTH_8  = 1,
    PCIE_CFG_WIDTH_16 = 2,
    PCIE_CFG_WIDTH_32 = 4
} pcie_cfg_width_t;

pcie_status_t pcie_cfg_read(pcie_dev_t *dev,
                            uint8_t bus, uint8_t device, uint8_t function,
                            uint16_t reg, pcie_cfg_width_t width, uint32_t *out_val);

pcie_status_t pcie_cfg_write(pcie_dev_t *dev,
                             uint8_t bus, uint8_t device, uint8_t function,
                             uint16_t reg, pcie_cfg_width_t width, uint32_t value);

#ifdef __cplusplus
}
#endif

#endif /* PCIE_CFG_H */
