/*
 * Synopsys DesignWare PCIe (DM) v6.00a - PIPE PHY Interface HAL
 * Header
 */
#ifndef DW_PCIE_DM_PIPE_PHY_H
#define DW_PCIE_DM_PIPE_PHY_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    DW_PCIE_DM_OK = 0,
    DW_PCIE_DM_EINVAL = -1,
    DW_PCIE_DM_ENOTSUP = -2,
    DW_PCIE_DM_ENOTFOUND = -3,
    DW_PCIE_DM_EBUSY = -4,
    DW_PCIE_DM_ETIMEOUT = -5
} dw_pcie_dm_status_t;

/* Abstracted PIPE PHY bus accessors and controls */
typedef struct {
    /* Reset control (active level defined by platform) */
    void (*assert_reset)(void);
    void (*deassert_reset)(void);
    /* Optional refclk/power enables */
    void (*enable_refclk)(void);
    void (*disable_refclk)(void);
    void (*power_on)(void);
    void (*power_off)(void);
    /* Register bus access (optional, vendor-specific) */
    int32_t (*phy_reg_read)(uint32_t reg, uint32_t *val);
    int32_t (*phy_reg_write)(uint32_t reg, uint32_t val);
    /* Poll for PHY ready/PLL lock */
    bool (*poll_lock)(uint32_t timeout_us);
} dw_pcie_dm_pipe_phy_ops_t;

/* Context */
typedef struct {
    const dw_pcie_dm_pipe_phy_ops_t *ops; /* all members optional except reset/poll */
} dw_pcie_dm_pipe_phy_ctx_t;

/* Public API */
int32_t dw_pcie_dm_pipe_phy_init(dw_pcie_dm_pipe_phy_ctx_t *ctx, const dw_pcie_dm_pipe_phy_ops_t *ops);
int32_t dw_pcie_dm_pipe_phy_cold_boot(dw_pcie_dm_pipe_phy_ctx_t *ctx, uint32_t timeout_us);
int32_t dw_pcie_dm_pipe_phy_write_checked(dw_pcie_dm_pipe_phy_ctx_t *ctx, uint32_t reg, uint32_t mask, uint32_t value);

#ifdef __cplusplus
}
#endif

#endif /* DW_PCIE_DM_PIPE_PHY_H */
