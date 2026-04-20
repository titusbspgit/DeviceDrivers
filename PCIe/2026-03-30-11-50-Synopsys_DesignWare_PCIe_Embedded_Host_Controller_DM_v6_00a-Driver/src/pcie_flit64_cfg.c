/*
 * Synopsys DesignWare PCIe DM v6.00a - PCIe 6.0 Flit Mode (64.0 GT/s) helper
 * RCI-ag-DrvGen Agent
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "../include/pcie_flit64_cfg.h"

/* Internal: walk extended capabilities to find PL64G */
static uint32_t read32(const struct pcie_flit64_ctx *ctx, uint32_t off)
{
    return ctx->ops->cfg_read32(ctx->cookie, off);
}

static uint16_t read16(const struct pcie_flit64_ctx *ctx, uint32_t off)
{
    return pcie_cfg_read16(ctx->ops, ctx->cookie, off);
}

static void write16(const struct pcie_flit64_ctx *ctx, uint32_t off, uint16_t v)
{
    pcie_cfg_write16(ctx->ops, ctx->cookie, off, v);
}

int pcie_flit64_init(struct pcie_flit64_ctx *ctx,
                     void *cookie,
                     const struct pcie_cfg_ops *ops)
{
    if ((ctx == NULL) || (ops == NULL) || (ops->cfg_read32 == NULL) || (ops->cfg_write32 == NULL)) {
        return -1;
    }
    ctx->cookie = cookie;
    ctx->ops = ops;
    ctx->pl64g_base = 0u;

    /* Discover PCIe Capability base from 0x34 pointer */
    const uint8_t cap_ptr = pcie_cfg_read8(ops, cookie, PCIE_CFG_CAP_PTR_OFFS);
    ctx->pcie_cap_base = (uint16_t)cap_ptr;
    return 0;
}

int pcie_flit64_find_capabilities(struct pcie_flit64_ctx *ctx)
{
    if (ctx == NULL) {
        return -1;
    }

    /* Walk from 0x100 through next pointers to find PL64G (ID 0x0031) */
    uint32_t off = 0x100u;
    ctx->pl64g_base = 0u;
    while ((off >= 0x100u) && (off < 0x1000u)) {
        const uint32_t hdr = read32(ctx, off + 0x0u);
        const uint16_t id = (uint16_t)(hdr & 0xFFFFu);
        const uint32_t next = (hdr >> 20) & 0xFFFu;
        if (id == PCIE_EXT_CAP_ID_PL64G) {
            ctx->pl64g_base = off;
        }
        if (next == 0u) {
            break;
        }
        off = next;
    }
    return 0;
}

int pcie_flit64_get_status(struct pcie_flit64_ctx *ctx,
                           bool *flit_supported,
                           bool *flit_enabled,
                           bool *flit_active,
                           uint8_t *negotiated_speed)
{
    if ((ctx == NULL) || (flit_supported == NULL) || (flit_enabled == NULL) ||
        (flit_active == NULL) || (negotiated_speed == NULL)) {
        return -1;
    }

    const uint32_t base = (uint32_t)ctx->pcie_cap_base;
    const uint16_t lnksts = read16(ctx, base + PCIE_CAP_REG_LINK_STATUS);
    *negotiated_speed = (uint8_t)((lnksts & PCIE_LINK_STATUS_SPEED_MASK) >> PCIE_LINK_STATUS_SPEED_SHIFT);

    /* DevCtl3/DevSts3 pair is at +0x38 (lo16/hi16) by PCIe 6.0 spec */
    const uint16_t devctl3 = read16(ctx, base + PCIE_OFFS_DEVCTL3_DEVSTS3);
    const uint16_t devsts3 = read16(ctx, base + PCIE_OFFS_DEVCTL3_DEVSTS3 + 2u);

    *flit_enabled = ((devctl3 & PCIE_DEVCTL3_FLIT_ENABLE) != 0u);

    /* Flit active status is normative in Link Status 2 bit10 (upper16 of 0x30) */
    const uint16_t lnksts2 = read16(ctx, base + PCIE_OFFS_LNKCTL2_LNKSTS2 + 2u);
    *flit_active = ((lnksts2 & PCIE_LNKSTS2_FLIT_ACTIVE) != 0u);

    /* Support is indicated either by capability header bit31 or presence of PL64G cap */
    const uint32_t cap_hdr_dw0 = read32(ctx, (uint32_t)ctx->pcie_cap_base + 0x0u);
    const bool cap_hdr_flit = ((cap_hdr_dw0 & PCIE_CAPS_HDR_FLIT_SUPPORTED) != 0u);
    *flit_supported = cap_hdr_flit || (ctx->pl64g_base != 0u);

    (void)devsts3; /* reserved for future use, mirrored by LnkSts2 */

    return 0;
}

int pcie_flit64_request_enable(struct pcie_flit64_ctx *ctx, bool enable,
                               bool retrain_link,
                               uint32_t timeout_us)
{
    if (ctx == NULL) {
        return -1;
    }

    const uint32_t base = (uint32_t)ctx->pcie_cap_base;

    /* Program DevCtl3 enable */
    uint16_t devctl3 = read16(ctx, base + PCIE_OFFS_DEVCTL3_DEVSTS3);
    if (enable) {
        devctl3 = (uint16_t)(devctl3 | PCIE_DEVCTL3_FLIT_ENABLE);
    } else {
        devctl3 = (uint16_t)(devctl3 & (uint16_t)~PCIE_DEVCTL3_FLIT_ENABLE);
    }
    write16(ctx, base + PCIE_OFFS_DEVCTL3_DEVSTS3, devctl3);

    if (retrain_link) {
        /* If enabling, select 64.0 GT/s target */
        if (enable) {
            uint16_t lnkctl2 = read16(ctx, base + PCIE_OFFS_LNKCTL2_LNKSTS2);
            lnkctl2 = (uint16_t)((lnkctl2 & (uint16_t)~PCIE_LNKCTL2_TARGET_SPEED_MASK) | PCIE_LNKCTL2_TARGET_SPEED_64G);
            write16(ctx, base + PCIE_OFFS_LNKCTL2_LNKSTS2, lnkctl2);
        }
        /* Set Retrain */
        uint16_t lnkctl = read16(ctx, base + PCIE_CAP_REG_LINK_CONTROL);
        lnkctl = (uint16_t)(lnkctl | PCIE_LINK_CONTROL_RETRAIN);
        write16(ctx, base + PCIE_CAP_REG_LINK_CONTROL, lnkctl);

        /* Poll for training complete and DLLLA, then optional flit active */
        uint32_t remaining = timeout_us;
        const uint32_t step_us = 100u; /* 100us granularity */
        while (remaining > 0u) {
            const uint16_t st = read16(ctx, base + PCIE_CAP_REG_LINK_STATUS);
            const bool training = ((st & PCIE_LINK_STATUS_TRAINING) != 0u);
            const bool dllla = ((st & PCIE_LINK_STATUS_DLLLA) != 0u);
            if ((!training) && dllla) {
                break;
            }
            /* Busy-wait delay hook: integration should implement precise wait if needed */
            /* In bare-metal/RTOS integrate a udelay; here, just decrement. */
            if (remaining < step_us) { remaining = 0u; } else { remaining -= step_us; }
        }
        if (remaining == 0u) {
            return -2; /* timeout */
        }

        /* If enabling, confirm negotiated speed and flit active within remaining timeout */
        if (enable) {
            remaining = timeout_us;
            while (remaining > 0u) {
                const uint16_t st = read16(ctx, base + PCIE_CAP_REG_LINK_STATUS);
                const uint8_t spd = (uint8_t)(st & PCIE_LINK_STATUS_SPEED_MASK);
                const uint16_t l2 = read16(ctx, base + PCIE_OFFS_LNKCTL2_LNKSTS2 + 2u);
                const bool flit = ((l2 & PCIE_LNKSTS2_FLIT_ACTIVE) != 0u);
                if ((spd == (uint8_t)PCIE_LNKCTL2_TARGET_SPEED_64G) && flit) {
                    break;
                }
                if (remaining < step_us) { remaining = 0u; } else { remaining -= step_us; }
            }
            if (remaining == 0u) {
                return -3; /* flit not active or speed not 64G within timeout */
            }
        }
    }

    return 0;
}
