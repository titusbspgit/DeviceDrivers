/*
 * Synopsys DesignWare PCIe (DM) v6.00a - PIPE PHY Interface HAL
 * Source
 */
#include "dw_pcie_dm_pipe_phy.h"

static inline void safe_call_void(void (*fn)(void))
{
    if (fn != (void *)0) {
        fn();
    }
}

int32_t dw_pcie_dm_pipe_phy_init(dw_pcie_dm_pipe_phy_ctx_t *ctx, const dw_pcie_dm_pipe_phy_ops_t *ops)
{
    if ((ctx == (void *)0) || (ops == (void *)0)) {
        return DW_PCIE_DM_EINVAL;
    }
    ctx->ops = ops;
    return DW_PCIE_DM_OK;
}

int32_t dw_pcie_dm_pipe_phy_cold_boot(dw_pcie_dm_pipe_phy_ctx_t *ctx, uint32_t timeout_us)
{
    if ((ctx == (void *)0) || (ctx->ops == (void *)0)) {
        return DW_PCIE_DM_EINVAL;
    }

    safe_call_void(ctx->ops->power_off);
    safe_call_void(ctx->ops->disable_refclk);

    if (ctx->ops->assert_reset != (void *)0) {
        ctx->ops->assert_reset();
    } else {
        return DW_PCIE_DM_EINVAL;
    }

    /* Power and clock up in order */
    safe_call_void(ctx->ops->enable_refclk);
    safe_call_void(ctx->ops->power_on);

    if (ctx->ops->deassert_reset != (void *)0) {
        ctx->ops->deassert_reset();
    } else {
        return DW_PCIE_DM_EINVAL;
    }

    /* Wait for PHY lock */
    if (ctx->ops->poll_lock != (void *)0) {
        const bool locked = ctx->ops->poll_lock(timeout_us);
        return locked ? DW_PCIE_DM_OK : DW_PCIE_DM_ETIMEOUT;
    }
    return DW_PCIE_DM_ENOTSUP;
}

int32_t dw_pcie_dm_pipe_phy_write_checked(dw_pcie_dm_pipe_phy_ctx_t *ctx, uint32_t reg, uint32_t mask, uint32_t value)
{
    if ((ctx == (void *)0) || (ctx->ops == (void *)0) || (ctx->ops->phy_reg_read == (void *)0) || (ctx->ops->phy_reg_write == (void *)0)) {
        return DW_PCIE_DM_ENOTSUP;
    }
    if ((mask == 0u) || ((value & ~mask) != 0u)) {
        return DW_PCIE_DM_EINVAL;
    }
    uint32_t cur = 0u;
    int32_t rc = ctx->ops->phy_reg_read(reg, &cur);
    if (rc != DW_PCIE_DM_OK) { return rc; }
    const uint32_t newv = (cur & ~mask) | (value & mask);
    rc = ctx->ops->phy_reg_write(reg, newv);
    if (rc != DW_PCIE_DM_OK) { return rc; }
    uint32_t rdback = 0u;
    rc = ctx->ops->phy_reg_read(reg, &rdback);
    if (rc != DW_PCIE_DM_OK) { return rc; }
    return (rdback == newv) ? DW_PCIE_DM_OK : DW_PCIE_DM_EBUSY;
}
