/*
 * Synopsys DesignWare PCIe (DM) v6.00a - PCIe 6.0 64.0 GT/s FLIT Mode Support (PL64G)
 * Header
 *
 * RCI-ag-DrvGen Agent
 */
#ifndef DW_PCIE_DM_PCIE6_FLIT_MODE_H
#define DW_PCIE_DM_PCIE6_FLIT_MODE_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Compile-time endianness selection: define DW_PCIE_DM_BIG_ENDIAN if needed */
#ifndef DW_PCIE_DM_BIG_ENDIAN
#define DW_PCIE_DM_LE32(x)   (x)
#define DW_PCIE_DM_BE32(x)   __builtin_bswap32((uint32_t)(x))
#define DW_PCIE_DM_CPU_TO_LE32(x) ((uint32_t)(x))
#define DW_PCIE_DM_LE32_TO_CPU(x) ((uint32_t)(x))
#else
#define DW_PCIE_DM_LE32(x)   __builtin_bswap32((uint32_t)(x))
#define DW_PCIE_DM_BE32(x)   (x)
#define DW_PCIE_DM_CPU_TO_LE32(x) (__builtin_bswap32((uint32_t)(x)))
#define DW_PCIE_DM_LE32_TO_CPU(x) (__builtin_bswap32((uint32_t)(x)))
#endif

/* Error codes */
typedef enum {
    DW_PCIE_DM_OK = 0,
    DW_PCIE_DM_EINVAL = -1,
    DW_PCIE_DM_ENOTSUP = -2,
    DW_PCIE_DM_ENOTFOUND = -3,
    DW_PCIE_DM_EBUSY = -4,
    DW_PCIE_DM_ETIMEOUT = -5
} dw_pcie_dm_status_t;

/* Context for local PF0 DBI-config space access */
typedef struct {
    volatile uint8_t *dbi_base; /* Base of DBI window mapping PF0 config space */
} dw_pcie_dm_pl64g_ctx_t;

/* PCIe Extended Capability IDs */
#define PCIE_EXT_CAP_ID_PL64G               (0x0031u)

/* PL64G register offsets relative to PL64G capability base */
#define PL64G_OFF_CAPABILITY                (0x04u)
#define PL64G_OFF_CONTROL                   (0x08u)
#define PL64G_OFF_STATUS                    (0x0Cu)

/* PL64G STATUS bits */
#define PL64G_STS_TX_PRECODING_REQ          (1u << 6)
#define PL64G_STS_TX_PRECODING_ON           (1u << 5)
#define PL64G_STS_EQ_64G_REQ                (1u << 4)
#define PL64G_STS_EQ_64G_CPL_P3             (1u << 3)
#define PL64G_STS_EQ_64G_CPL_P2             (1u << 2)
#define PL64G_STS_EQ_64G_CPL_P1             (1u << 1)
#define PL64G_STS_EQ_64G_CPL                (1u << 0)

/* Public API */
int32_t dw_pcie_dm_pl64g_init(dw_pcie_dm_pl64g_ctx_t *ctx, uintptr_t dbi_base);
int32_t dw_pcie_dm_pl64g_is_present(const dw_pcie_dm_pl64g_ctx_t *ctx, bool *present);
int32_t dw_pcie_dm_pl64g_read_status(const dw_pcie_dm_pl64g_ctx_t *ctx, uint32_t *status);
int32_t dw_pcie_dm_pl64g_wait_eq_complete(const dw_pcie_dm_pl64g_ctx_t *ctx, uint32_t timeout_us, bool *completed);
int32_t dw_pcie_dm_pl64g_precoding_status(const dw_pcie_dm_pl64g_ctx_t *ctx, bool *required, bool *enabled);

#ifdef __cplusplus
}
#endif

#endif /* DW_PCIE_DM_PCIE6_FLIT_MODE_H */
