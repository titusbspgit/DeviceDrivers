/*
 * Synopsys DesignWare PCIe DM v6.00a - PCIe 6.0 Flit Mode (64.0 GT/s) helper
 * RCI-ag-DrvGen Agent
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "../include/pcie_flit64_cfg.h"

/* Standard PCI configuration space offsets */
#define PCIE_CFG_CAP_PTR_OFFS                (0x34u)

/* Extended Capability discovery start */
#define PCIE_CFG_EXT_CAP_START               (0x100u)

/* Helpers */
static uint16_t read_cap16(const struct pcie_flit64_ctx *ctx, uint32_t off)
{
    return pcie_cfg_read16(ctx->ops, ctx->cookie, (uint32_t)ctx->pcie_cap_base + off);
}

static void write_cap16(const struct pcie_flit64_ctx *ctx, uint32_t off, uint16_t val)
{
    pcie_cfg_write16(ctx->ops, ctx->cookie, (uint32_t)ctx->pcie_cap_base + off, val);
}

static uint32_t read_excap32(const struct pcie_flit64_ctx *ctx, uint32_t base, uint32_t off)
{
    return ctx->ops->cfg_read32(ctx->cookie, base + off);
}

static void write_excap32(const struct pcie_flit64_ctx *ctx, uint32_t base, uint32_t off, uint32_t val)
{
    ctx->ops->cfg_write32(ctx->cookie, base + off, val);
}

static uint16_t read_excap16(const struct pcie_flit64_ctx *ctx, uint32_t base, uint32_t off)
{
    return pcie_cfg_read16(ctx->ops, ctx->cookie, base + off);
}

static void write_excap16(const struct pcie_flit64_ctx *ctx, uint32_t base, uint32_t off, uint16_t v)
{
    pcie_cfg_write16(ctx->ops, ctx->cookie, base + off, v);
}

static uint32_t get_extcap_next(uint32_t hdr)
{
    const uint32_t next = (hdr >> PCIE_EXT_CAP_HDR_NEXT_SHIFT) & PCIE_EXT_CAP_HDR_NEXT_MASK;
    return next;
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
    ctx->pcie_cap_base = 0u;
    ctx->extcap_base = PCIE_CFG_EXT_CAP_START;
    ctx->pl64g_base = 0u;

    /* Discover PCIe Capability pointer from standard config space */
    const uint8_t cap_ptr = pcie_cfg_read8(ops, cookie, PCIE_CFG_CAP_PTR_OFFS);
    ctx->pcie_cap_base = (uint16_t)cap_ptr; /* Type 0 assumed */

    return 0;
}

int pcie_flit64_find_capabilities(struct pcie_flit64_ctx *ctx)
{
    if (ctx == NULL) {
        return -1;
    }

    /* Walk extended capabilities to find PL64G and capture DevCtl3/DevSts3 block base */
    uint32_t off = ctx->extcap_base;
    ctx->pl64g_base = 0u;

    /* Safe bounded walk: max 4KB */
    while ((off >= PCIE_CFG_EXT_CAP_START) && (off < 0x1000u)) {
        const uint32_t hdr = read_excap32(ctx, off, 0u);
        const uint16_t id = (uint16_t)(hdr & PCIE_EXT_CAP_HDR_ID_MASK);
        if (id == PCIE_EXT_CAP_ID_PL64G) {
            ctx->pl64g_base = off;
        }
        const uint32_t next = get_extcap_next(hdr);
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

    /* Discover DevCtl3/DevSts3 by assumption at fixed offsets relative to an implementation block. */
    /* Since locations may vary, this helper reads at commonly used offsets within the PCIe 6.0 block. */
    const uint16_t lnksts = read_cap16(ctx, PCIE_CAP_REG_LINK_STATUS);
    *negotiated_speed = (uint8_t)((lnksts & PCIE_LINK_STATUS_SPEED_MASK) >> PCIE_LINK_STATUS_SPEED_SHIFT);

    const uint16_t devctl3 = read_excap16(ctx, (uint32_t)ctx->pcie_cap_base, PCIE_DEVCTL3_OFFSET);
    const uint16_t devsts3 = read_excap16(ctx, (uint32_t)ctx->pcie_cap_base, PCIE_DEVSTS3_OFFSET);

    *flit_enabled = ((devctl3 & PCIE_DEVCTL3_FLIT_ENABLE) != 0u);
    *flit_active  = ((devsts3 & PCIE_DEVSTS3_FLIT_ACTIVE) != 0u);

    /* Assume capability supported if PL64G cap is present or DevSts3 reports valid */
    *flit_supported = (ctx->pl64g_base != 0u) || (*flit_active) || (*flit_enabled);

    return 0;
}

int pcie_flit64_request_enable(struct pcie_flit64_ctx *ctx, bool enable,
                               bool retrain_link)
{
    if (ctx == NULL) {
        return -1;
    }

    uint16_t devctl3 = read_excap16(ctx, (uint32_t)ctx->pcie_cap_base, PCIE_DEVCTL3_OFFSET);

    if (enable) {
        devctl3 = (uint16_t)(devctl3 | PCIE_DEVCTL3_FLIT_ENABLE);
    } else {
        devctl3 = (uint16_t)(devctl3 & (uint16_t)~PCIE_DEVCTL3_FLIT_ENABLE);
    }
    write_excap16(ctx, (uint32_t)ctx->pcie_cap_base, PCIE_DEVCTL3_OFFSET, devctl3);

    if (retrain_link) {
        /* Set target speed if enabling 64G; leave unchanged if disabling */
        if (enable) {
            uint16_t lnkctl2 = read_excap16(ctx, (uint32_t)ctx->pcie_cap_base, PCIE_LNKCTL2_OFFSET);
            lnkctl2 = (uint16_t)((lnkctl2 & (uint16_t)~PCIE_LNKCTL2_TARGET_SPEED_MASK) | PCIE_LNKCTL2_TARGET_SPEED_64G);
            write_excap16(ctx, (uint32_t)ctx->pcie_cap_base, PCIE_LNKCTL2_OFFSET, lnkctl2);
        }
        /* Retrain */
        uint16_t lnkctl = read_cap16(ctx, PCIE_CAP_REG_LINK_CONTROL);
        lnkctl = (uint16_t)(lnkctl | PCIE_LINK_CONTROL_RETRAIN);
        write_cap16(ctx, PCIE_CAP_REG_LINK_CONTROL, lnkctl);
    }

    return 0;
}
