/* Feature: Initialization sequence (RC mode) */
#ifndef KEYSTONE_PCIE_MODULE_INITIALIZATION_SEQUENCE_RC_MODE_H
#define KEYSTONE_PCIE_MODULE_INITIALIZATION_SEQUENCE_RC_MODE_H

#include <stdint.h>
#include <stdbool.h>
#include "core_driver.h"

#ifdef __cplusplus
extern "C" {
#endif

struct kpm_init_rc_params {
    bool request_gen2;     /* true: request Gen2 speed */
    bool common_clock;     /* true: set common clock */
};

int kpm_init_rc_sequence(const struct pcie_core_api *api, const struct kpm_init_rc_params *p);

#ifdef __cplusplus
}
#endif

#endif
