/* Feature: MSI generation (EP mode) */
#ifndef KEYSTONE_PCIE_MODULE_MSI_GENERATION_EP_MODE_H
#define KEYSTONE_PCIE_MODULE_MSI_GENERATION_EP_MODE_H

#include <stdint.h>
#include "core_driver.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Program local MSI capability target (host-provided). */
int kpm_msi_program_target(const struct pcie_core_api *api, uint32_t addr_low, uint32_t addr_up, uint16_t data);

/* Triggering MSI typically requires performing a posted write to the host MSI address.
 * This path is platform-specific; not implemented here. */
static inline int kpm_msi_trigger_vector(const struct pcie_core_api *api, uint32_t vec)
{ (void)api; (void)vec; return -1; }

#ifdef __cplusplus
}
#endif

#endif
