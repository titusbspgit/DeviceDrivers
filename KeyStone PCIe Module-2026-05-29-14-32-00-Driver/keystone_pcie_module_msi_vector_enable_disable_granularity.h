/*
 * KeyStone PCIe Module - MSI vector enable/disable granularity
 * Author: ag-drv-CodeGen Agent
 * Date: 2026-05-29
 * Category: Interrupts
 * Description: Enable/disable and status handling for MSI vectors. Programming details UNKNOWN.
 * Dependencies: MSIx capability/registers (enable/set/clear/status) - addresses/bitfields UNKNOWN.
 */
#ifndef KEYSTONE_PCIE_MODULE_MSI_VECTOR_ENABLE_DISABLE_GRANULARITY_H
#define KEYSTONE_PCIE_MODULE_MSI_VECTOR_ENABLE_DISABLE_GRANULARITY_H

#include <stdint.h>
#include <stdbool.h>
#include "core_keystone_pcie_module.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    KPM_Core *core;
} kpm_msi_ctx_t;

/* Initialize MSI control context */
kpm_status_t kpm_msi_init(kpm_msi_ctx_t *ctx, KPM_Core *core);

/* Enable a specific MSI vector [0..31] (UNKNOWN mapping) */
kpm_status_t kpm_msi_enable_vector(kpm_msi_ctx_t *ctx, uint8_t vector);

/* Disable a specific MSI vector [0..31] (UNKNOWN mapping) */
kpm_status_t kpm_msi_disable_vector(kpm_msi_ctx_t *ctx, uint8_t vector);

/* Check if a vector is pending (UNKNOWN mapping) */
kpm_status_t kpm_msi_is_pending(kpm_msi_ctx_t *ctx, uint8_t vector, bool *pending);

/* Acknowledge/clear a vector (UNKNOWN mapping) */
kpm_status_t kpm_msi_ack(kpm_msi_ctx_t *ctx, uint8_t vector);

#ifdef __cplusplus
}
#endif

#endif /* KEYSTONE_PCIE_MODULE_MSI_VECTOR_ENABLE_DISABLE_GRANULARITY_H */
