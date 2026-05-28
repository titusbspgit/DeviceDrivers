/* Feature: Common clock and SSC configuration */
#ifndef KEYSTONE_PCIE_MODULE_COMMON_CLOCK_AND_SSC_CONFIGURATION_H
#define KEYSTONE_PCIE_MODULE_COMMON_CLOCK_AND_SSC_CONFIGURATION_H

#include <stdint.h>
#include <stdbool.h>
#include "core_driver.h"

#ifdef __cplusplus
extern "C" {
#endif

int kpm_set_common_clock(const struct pcie_core_api *api, bool enable);
bool kpm_get_common_clock(const struct pcie_core_api *api);

#ifdef __cplusplus
}
#endif

#endif
