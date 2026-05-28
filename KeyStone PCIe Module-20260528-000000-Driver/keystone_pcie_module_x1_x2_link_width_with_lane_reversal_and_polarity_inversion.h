/* Feature: x1/x2 link width with lane reversal and polarity inversion */
#ifndef KEYSTONE_PCIE_MODULE_X1_X2_LINK_WIDTH_WITH_LANE_REVERSAL_AND_POLARITY_INVERSION_H
#define KEYSTONE_PCIE_MODULE_X1_X2_LINK_WIDTH_WITH_LANE_REVERSAL_AND_POLARITY_INVERSION_H

#include <stdint.h>
#include <stdbool.h>
#include "core_driver.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Disable or enable automatic width downtraining control */
int kpm_set_auto_width_downtrain_disable(const struct pcie_core_api *api, bool disable);

/* Read negotiated width (x1/x2) from status */
uint32_t kpm_get_negotiated_width(const struct pcie_core_api *api);

#ifdef __cplusplus
}
#endif

#endif
