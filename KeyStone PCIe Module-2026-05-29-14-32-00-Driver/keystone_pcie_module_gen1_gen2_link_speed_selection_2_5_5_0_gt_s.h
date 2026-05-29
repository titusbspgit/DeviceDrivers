/*
 * KeyStone PCIe Module - Gen1/Gen2 link speed selection (2.5/5.0 GT/s)
 * Author: ag-drv-CodeGen Agent
 * Date: 2026-05-29
 * Category: Performance
 * Description: Operates at 2.5 GT/s (Gen1) or 5.0 GT/s (Gen2). Programming details UNKNOWN.
 * Dependencies: SerDes PLL/config; LINK_CTRL2 speed; Stable REFCLK; LTSSM enabled (addresses/bitfields UNKNOWN)
 */
#ifndef KEYSTONE_PCIE_MODULE_GEN1_GEN2_LINK_SPEED_SELECTION_2_5_5_0_GT_S_H
#define KEYSTONE_PCIE_MODULE_GEN1_GEN2_LINK_SPEED_SELECTION_2_5_5_0_GT_S_H

#include <stdint.h>
#include <stdbool.h>
#include "core_keystone_pcie_module.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    KPM_LINK_SPEED_GEN1 = 0, /* 2.5 GT/s */
    KPM_LINK_SPEED_GEN2 = 1  /* 5.0 GT/s */
} kpm_link_speed_t;

/* Opaque driver context */
typedef struct {
    KPM_Core *core;
} kpm_linkspeed_ctx_t;

/* Initialize link speed control context */
kpm_status_t kpm_linkspeed_init(kpm_linkspeed_ctx_t *ctx, KPM_Core *core);

/* Configure target link speed (requires LTSSM disabled beforehand). */
kpm_status_t kpm_linkspeed_set(kpm_linkspeed_ctx_t *ctx, kpm_link_speed_t speed);

/* Read back negotiated/target link speed (UNKNOWN register mapping). */
kpm_status_t kpm_linkspeed_get(kpm_linkspeed_ctx_t *ctx, kpm_link_speed_t *speed_out);

/* Control link training state machine (abstract, UNKNOWN mapping). */
kpm_status_t kpm_linkspeed_training_enable(kpm_linkspeed_ctx_t *ctx, bool enable);

#ifdef __cplusplus
}
#endif

#endif /* KEYSTONE_PCIE_MODULE_GEN1_GEN2_LINK_SPEED_SELECTION_2_5_5_0_GT_S_H */
