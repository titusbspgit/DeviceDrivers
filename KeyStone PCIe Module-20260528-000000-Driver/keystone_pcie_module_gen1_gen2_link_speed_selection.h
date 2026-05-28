/* Feature: Gen1/Gen2 link speed selection */
#ifndef KEYSTONE_PCIE_MODULE_GEN1_GEN2_LINK_SPEED_SELECTION_H
#define KEYSTONE_PCIE_MODULE_GEN1_GEN2_LINK_SPEED_SELECTION_H

#include <stdint.h>
#include <stdbool.h>
#include "core_driver.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    KPM_LINK_GEN1 = 1,
    KPM_LINK_GEN2 = 2,
} kpm_link_speed_t;

int kpm_set_target_link_speed(const struct pcie_core_api *api, kpm_link_speed_t gen);
int kpm_retrain_link_and_wait(const struct pcie_core_api *api, uint32_t timeout_loops);
uint32_t kpm_get_negotiated_speed(const struct pcie_core_api *api);

#ifdef __cplusplus
}
#endif

#endif
