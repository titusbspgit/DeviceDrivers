#ifndef KEYSTONE_PCIE_MODULE_LINK_WIDTH_AND_SPEED_X1_X2_GEN1_GEN2_H
#define KEYSTONE_PCIE_MODULE_LINK_WIDTH_AND_SPEED_X1_X2_GEN1_GEN2_H

#include <stdint.h>
#include <stdbool.h>
#include "core_keystone_pcie_module.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Registers for link width/speed control are UNKNOWN in current spec. */

typedef enum {
    KPM_PCIE_LINK_WIDTH_X1 = 1,
    KPM_PCIE_LINK_WIDTH_X2 = 2,
} kpm_pcie_link_width_t;

typedef enum {
    KPM_PCIE_GEN1 = 1,
    KPM_PCIE_GEN2 = 2,
} kpm_pcie_gen_t;

typedef struct {
    kpm_pcie_link_width_t width;
    kpm_pcie_gen_t        gen;
} kpm_pcie_link_cfg_t;

kpm_status_t kpm_pcie_link_configure(const kpm_core_api_t *api, const kpm_pcie_link_cfg_t *cfg);

kpm_status_t kpm_pcie_link_get_status(const kpm_core_api_t *api, kpm_pcie_link_cfg_t *status_out);

#ifdef __cplusplus
}
#endif

#endif /* KEYSTONE_PCIE_MODULE_LINK_WIDTH_AND_SPEED_X1_X2_GEN1_GEN2_H */
