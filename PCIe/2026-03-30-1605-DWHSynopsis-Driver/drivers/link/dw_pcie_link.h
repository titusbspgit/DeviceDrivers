#ifndef DW_PCIE_LINK_H
#define DW_PCIE_LINK_H

#include <stdint.h>
#include <stdbool.h>
#include "dw_pcie_hal.h"
#include "dw_pcie_regs.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Initialize link-related state (ensures PCIe capability cached) */
int dw_pcie_link_init(dw_pcie_dev_t *dev);

/* Get negotiated link speed (1..N) and width (x1,x2,x4,...) */
int dw_pcie_link_get_status(dw_pcie_dev_t *dev, uint8_t *speed, uint8_t *width);

/* Program target link speed (1=Gen1,2=Gen2,3=Gen3,...) and request directed speed change */
int dw_pcie_link_set_target_speed(dw_pcie_dev_t *dev, uint8_t target_speed);

/* Retrain the link and wait until training completes or timeout expires (polling loops) */
int dw_pcie_link_retrain_wait(dw_pcie_dev_t *dev, uint32_t timeout_iters);

#ifdef __cplusplus
}
#endif

#endif /* DW_PCIE_LINK_H */
