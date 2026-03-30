/*
 * MIT License
 * Secondary PCIe Extended Capability (SPCIE)
 */
#ifndef PCIE_SPCIE_H
#define PCIE_SPCIE_H

#include "drivers/pcie/common/pcie_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    uint16_t link_control3;
    uint16_t link_status3;
} pcie_spcie_link3_t;

pcie_status_t pcie_spcie_get_link3(const pcie_dbi_t* ctx, pcie_spcie_link3_t* out);
pcie_status_t pcie_spcie_set_link_control3(pcie_dbi_t* ctx, uint16_t link_control3);

pcie_status_t pcie_spcie_get_lane_err_status(const pcie_dbi_t* ctx, uint32_t* out_mask);
pcie_status_t pcie_spcie_clear_lane_err_status(pcie_dbi_t* ctx, uint32_t clear_mask);

#ifdef __cplusplus
}
#endif

#endif /* PCIE_SPCIE_H */
