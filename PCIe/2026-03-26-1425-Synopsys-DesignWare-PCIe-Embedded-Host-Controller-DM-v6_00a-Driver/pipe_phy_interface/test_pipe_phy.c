/* Test application for PIPE PHY HAL - uses a mock backend */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "dw_pcie_dm_pipe_phy.h"

/* Simple mock PHY implementation for validation */
static uint32_t mock_regs[16];
static bool locked = false;

static void mock_assert_reset(void) { locked = false; }
static void mock_deassert_reset(void) { /* nothing */ }
static void mock_enable_refclk(void) { /* nothing */ }
static void mock_disable_refclk(void) { /* nothing */ }
static void mock_power_on(void) { /* nothing */ }
static void mock_power_off(void) { /* nothing */ }
static bool mock_poll_lock(uint32_t timeout_us) { (void)timeout_us; locked = true; return true; }
static int32_t mock_read(uint32_t reg, uint32_t *val)
{
    if ((val == NULL) || (reg >= (uint32_t)(sizeof(mock_regs)/sizeof(mock_regs[0])))) { return DW_PCIE_DM_EINVAL; }
    *val = mock_regs[reg];
    return DW_PCIE_DM_OK;
}
static int32_t mock_write(uint32_t reg, uint32_t val)
{
    if (reg >= (uint32_t)(sizeof(mock_regs)/sizeof(mock_regs[0]))) { return DW_PCIE_DM_EINVAL; }
    mock_regs[reg] = val;
    return DW_PCIE_DM_OK;
}

int main(void)
{
    dw_pcie_dm_pipe_phy_ops_t ops = {
        .assert_reset = mock_assert_reset,
        .deassert_reset = mock_deassert_reset,
        .enable_refclk = mock_enable_refclk,
        .disable_refclk = mock_disable_refclk,
        .power_on = mock_power_on,
        .power_off = mock_power_off,
        .phy_reg_read = mock_read,
        .phy_reg_write = mock_write,
        .poll_lock = mock_poll_lock
    };
    dw_pcie_dm_pipe_phy_ctx_t ctx;
    int32_t rc = dw_pcie_dm_pipe_phy_init(&ctx, &ops);
    printf("[PIPE] init rc=%d\n", (int)rc);
    rc = dw_pcie_dm_pipe_phy_cold_boot(&ctx, 10000u);
    printf("[PIPE] cold_boot rc=%d\n", (int)rc);
    rc = dw_pcie_dm_pipe_phy_write_checked(&ctx, 1u, 0x0000000Fu, 0x00000005u);
    printf("[PIPE] write_checked rc=%d\n", (int)rc);
    uint32_t v = 0u; (void)mock_read(1u, &v);
    printf("[PIPE] reg1=0x%08x (expect 0x00000005)\n", (unsigned)v);
    return 0;
}
