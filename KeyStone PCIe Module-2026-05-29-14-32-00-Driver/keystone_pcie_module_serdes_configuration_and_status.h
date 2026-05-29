/*
 * KeyStone PCIe Module - SerDes configuration and status
 * Author: ag-drv-CodeGen Agent
 * Date: 2026-05-29
 * Category: SerDes/PHY
 * Description: Configure SerDes PLL and lanes; poll status. Programming details UNKNOWN.
 * Dependencies: PCIE_SERDES_CFGPLL; PCIE_SERDES_STS[LOCK]; PCS_CFG0/1; PCS_STATUS (addresses/bitfields UNKNOWN)
 */
#ifndef KEYSTONE_PCIE_MODULE_SERDES_CONFIGURATION_AND_STATUS_H
#define KEYSTONE_PCIE_MODULE_SERDES_CONFIGURATION_AND_STATUS_H

#include <stdint.h>
#include <stdbool.h>
#include "core_keystone_pcie_module.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    KPM_Core *core;
} kpm_serdes_ctx_t;

typedef struct {
    /* Placeholder PLL parameters; values are UNKNOWN */
    uint32_t pll_mpy;   /* UNKNOWN */
    uint32_t pll_cfg;   /* UNKNOWN */
} kpm_serdes_pll_cfg_t;

/* Initialize SerDes context */
kpm_status_t kpm_serdes_init(kpm_serdes_ctx_t *ctx, KPM_Core *core);

/* Configure PLL (UNKNOWN mapping) */
kpm_status_t kpm_serdes_configure_pll(kpm_serdes_ctx_t *ctx, const kpm_serdes_pll_cfg_t *cfg);

/* Wait for PLL lock with timeout (UNKNOWN mapping) */
kpm_status_t kpm_serdes_wait_lock(kpm_serdes_ctx_t *ctx, uint32_t timeout_us);

/* Enable/disable a lane (UNKNOWN mapping) */
kpm_status_t kpm_serdes_enable_lane(kpm_serdes_ctx_t *ctx, uint8_t lane, bool enable);

#ifdef __cplusplus
}
#endif

#endif /* KEYSTONE_PCIE_MODULE_SERDES_CONFIGURATION_AND_STATUS_H */
