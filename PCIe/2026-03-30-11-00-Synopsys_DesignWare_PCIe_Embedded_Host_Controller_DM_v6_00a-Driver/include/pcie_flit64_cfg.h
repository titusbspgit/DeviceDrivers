/*
 * Synopsys DesignWare PCIe DM v6.00a - PCIe 6.0 Flit Mode (64.0 GT/s) helper
 * RCI-ag-DrvGen Agent
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef PCIE_FLIT64_CFG_H
#define PCIE_FLIT64_CFG_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * This header provides minimal, portable register definitions and APIs for
 * discovering and validating PCIe 6.0 Flit Mode on a Synopsys DWC PCIe DM
 * controller instance. It assumes access to PCIe configuration space via
 * standard config read/write callbacks supplied by the integration layer.
 */

/* Extended Capability IDs */
#define PCIE_EXT_CAP_ID_PL64G                (0x0031u)

/* Standard PCI Express Capability offsets (relative to PCIe Cap base) */
#define PCIE_CAP_REG_LINK_CONTROL            (0x10u)
#define PCIE_CAP_REG_LINK_STATUS             (0x12u)

/* PCIe Capability: Link Control (offset 0x10) */
#define PCIE_LINK_CONTROL_RETRAIN            (0x0020u)

/* PCIe Capability: Link Status (offset 0x12) */
#define PCIE_LINK_STATUS_SPEED_MASK          (0x000Fu)
#define PCIE_LINK_STATUS_SPEED_SHIFT         (0u)

/* PCIe Extended Capability: Link Capabilities 2/Control 2/Status 2 (PCIe 2.x+) */
#define PCIE_EXT_CAP_ID_PCIEXP               (0x0001u)

/* Common Extended Capability header fields */
#define PCIE_EXT_CAP_HDR_ID_MASK             (0xFFFFu)
#define PCIE_EXT_CAP_HDR_VER_SHIFT           (16u)
#define PCIE_EXT_CAP_HDR_NEXT_SHIFT          (20u)
#define PCIE_EXT_CAP_HDR_NEXT_MASK           (0xFFFu)

/* Device/Link x3 (PCIe 6.0) register offsets within their extended capability
 * The exact placement depends on implementation; this interface discovers them
 * via standard PCIe navigation and expects the following field semantics.
 */
#define PCIE_DEVCTL3_OFFSET                  (0x20u) /* Assumed typical within the PCIe 6.0 block */
#define PCIE_DEVSTS3_OFFSET                  (0x22u)
#define PCIE_LNKCTL2_OFFSET                  (0x30u) /* Target Link Speed field present */

/* Device Control 3 bits */
#define PCIE_DEVCTL3_FLIT_ENABLE            (0x0001u) /* Bit0 assumed */

/* Device Status 3 bits */
#define PCIE_DEVSTS3_FLIT_ACTIVE            (0x0001u) /* Bit0 assumed */

/* Link Control 2: Target Link Speed [3:0] */
#define PCIE_LNKCTL2_TARGET_SPEED_MASK       (0x000Fu)
#define PCIE_LNKCTL2_TARGET_SPEED_64G        (0x0006u)

/* PL64G Extended Capability register offsets (relative to its cap base B) */
#define PL64G_OFFS_CAP_HDR                   (0x00u)
#define PL64G_OFFS_CAPABILITY                (0x04u)
#define PL64G_OFFS_CONTROL                   (0x08u)
#define PL64G_OFFS_STATUS                    (0x0Cu)

/* Integration layer opaque handle to perform config space accesses */
struct pcie_cfg_ops {
    uint32_t (*cfg_read32)(void *cookie, uint32_t offset);
    void     (*cfg_write32)(void *cookie, uint32_t offset, uint32_t value);
};

/* Driver context */
struct pcie_flit64_ctx {
    void *cookie;                 /* Passed to ops */
    const struct pcie_cfg_ops *ops; /* Config space accessors */
    uint16_t pcie_cap_base;       /* PCIe Capability pointer (type 0/1) */
    uint32_t extcap_base;         /* First Extended Capability pointer */
    uint32_t pl64g_base;          /* PL64G capability base if present, else 0 */
};

/* Public API */
int pcie_flit64_init(struct pcie_flit64_ctx *ctx,
                     void *cookie,
                     const struct pcie_cfg_ops *ops);

int pcie_flit64_find_capabilities(struct pcie_flit64_ctx *ctx);

int pcie_flit64_get_status(struct pcie_flit64_ctx *ctx,
                           bool *flit_supported,
                           bool *flit_enabled,
                           bool *flit_active,
                           uint8_t *negotiated_speed);

int pcie_flit64_request_enable(struct pcie_flit64_ctx *ctx, bool enable,
                               bool retrain_link);

/* Utility: 16-bit and 8-bit config access built on top of 32-bit ops */
static inline uint16_t pcie_cfg_read16(const struct pcie_cfg_ops *ops, void *cookie, uint32_t off)
{
    const uint32_t aligned = (off & ~0x3u);
    const uint32_t shift = (uint32_t)((off & 0x3u) * 8u);
    const uint32_t v = ops->cfg_read32(cookie, aligned);
    return (uint16_t)((v >> shift) & 0xFFFFu);
}

static inline void pcie_cfg_write16(const struct pcie_cfg_ops *ops, void *cookie, uint32_t off, uint16_t val)
{
    const uint32_t aligned = (off & ~0x3u);
    const uint32_t shift = (uint32_t)((off & 0x3u) * 8u);
    uint32_t v = ops->cfg_read32(cookie, aligned);
    const uint32_t mask = (0xFFFFu << shift);
    v = (v & ~mask) | (((uint32_t)val << shift) & mask);
    ops->cfg_write32(cookie, aligned, v);
}

static inline uint8_t pcie_cfg_read8(const struct pcie_cfg_ops *ops, void *cookie, uint32_t off)
{
    const uint32_t aligned = (off & ~0x3u);
    const uint32_t shift = (uint32_t)((off & 0x3u) * 8u);
    const uint32_t v = ops->cfg_read32(cookie, aligned);
    return (uint8_t)((v >> shift) & 0xFFu);
}

static inline void pcie_cfg_write8(const struct pcie_cfg_ops *ops, void *cookie, uint32_t off, uint8_t val)
{
    const uint32_t aligned = (off & ~0x3u);
    const uint32_t shift = (uint32_t)((off & 0x3u) * 8u);
    uint32_t v = ops->cfg_read32(cookie, aligned);
    const uint32_t mask = (0xFFu << shift);
    v = (v & ~mask) | (((uint32_t)val << shift) & mask);
    ops->cfg_write32(cookie, aligned, v);
}

#ifdef __cplusplus
}
#endif

#endif /* PCIE_FLIT64_CFG_H */
