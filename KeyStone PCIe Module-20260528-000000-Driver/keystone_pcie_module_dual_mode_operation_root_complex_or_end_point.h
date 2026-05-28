/* Feature: Dual-mode operation (Root Complex or End Point) */
#ifndef KEYSTONE_PCIE_MODULE_DUAL_MODE_OPERATION_ROOT_COMPLEX_OR_END_POINT_H
#define KEYSTONE_PCIE_MODULE_DUAL_MODE_OPERATION_ROOT_COMPLEX_OR_END_POINT_H

#include <stdint.h>
#include "core_driver.h"

#ifdef __cplusplus
extern "C" {
#endif

enum kpm_mode { KPM_MODE_UNKNOWN = 0, KPM_MODE_RC = 1, KPM_MODE_EP = 2 };

/* Read current operating mode. Hardware strap defines mode; no SW override here. */
static inline enum kpm_mode kpm_get_mode(const struct pcie_core_api *api)
{
    (void)api;
    /* UNKNOWN: no software-detectable override/register provided here. */
    return KPM_MODE_UNKNOWN; /* Missing data -> UNKNOWN */
}

/* Ensure mode is finalized before enabling LTSSM. No-op (documented constraint). */
static inline int kpm_mode_finalize_before_ltssm(const struct pcie_core_api *api)
{
    (void)api;
    return 0;
}

#ifdef __cplusplus
}
#endif

#endif
