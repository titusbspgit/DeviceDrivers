/* Feature: Link training control and LTSSM monitoring */
#ifndef KEYSTONE_PCIE_MODULE_LINK_TRAINING_CONTROL_AND_LTSSM_MONITORING_H
#define KEYSTONE_PCIE_MODULE_LINK_TRAINING_CONTROL_AND_LTSSM_MONITORING_H

#include <stdint.h>
#include <stdbool.h>
#include "core_driver.h"

#ifdef __cplusplus
extern "C" {
#endif

int kpm_ltssm_enable(const struct pcie_core_api *api, bool enable);
int kpm_wait_link_up(const struct pcie_core_api *api, uint32_t timeout_loops);

#ifdef __cplusplus
}
#endif

#endif
