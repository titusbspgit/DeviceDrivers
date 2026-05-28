/* Feature: MSI reception (RC mode) */
#ifndef KEYSTONE_PCIE_MODULE_MSI_RECEPTION_RC_MODE_H
#define KEYSTONE_PCIE_MODULE_MSI_RECEPTION_RC_MODE_H

#include <stdint.h>
#include <stdbool.h>
#include "core_driver.h"

#ifdef __cplusplus
extern "C" {
#endif

int kpm_msi_enable_vector(const struct pcie_core_api *api, uint32_t vec);
int kpm_msi_disable_vector(const struct pcie_core_api *api, uint32_t vec);
int kpm_msi_poll_and_clear(const struct pcie_core_api *api, uint32_t vec, bool *active);

#ifdef __cplusplus
}
#endif

#endif
