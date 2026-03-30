#ifndef DW_PCIE_MSI_H
#define DW_PCIE_MSI_H

#include <stdint.h>
#include <stdbool.h>
#include "dw_pcie_hal.h"
#include "dw_pcie_regs.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Configure MSI doorbell address (LO/HI) */
static inline void dw_pcie_msi_set_address(dw_pcie_dev_t *dev, uint64_t addr)
{
    dw_reg_write32(dev->dbi_base, DW_MSI_ADDR_LO_OFF, (uint32_t)(addr & 0xFFFFFFFFu));
    dw_reg_write32(dev->dbi_base, DW_MSI_ADDR_HI_OFF, (uint32_t)((addr >> 32) & 0xFFFFFFFFu));
}

/* Enable a vector number (0..31 in group 0, etc.). Returns 0 on success */
int dw_pcie_msi_enable_vector(dw_pcie_dev_t *dev, uint32_t vector);

/* Disable a vector */
int dw_pcie_msi_disable_vector(dw_pcie_dev_t *dev, uint32_t vector);

/* Read status bits for a vector group (group = vector/32). */
uint32_t dw_pcie_msi_read_status(dw_pcie_dev_t *dev, uint32_t group);

/* Clear status bits (W1C) for a vector group */
void dw_pcie_msi_clear_status(dw_pcie_dev_t *dev, uint32_t group, uint32_t mask);

#ifdef __cplusplus
}
#endif

#endif /* DW_PCIE_MSI_H */
