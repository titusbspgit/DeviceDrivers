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

/* Extended Capability IDs */
#define PCIE_EXT_CAP_ID_PL64G                (0x0031u)

/* Base PCIe Capability offsets (Type 0/1 header-relative) */
#define PCIE_CFG_CAP_PTR_OFFS                (0x34u)

/* PCIe Capability (base-relative) classic registers */
#define PCIE_CAP_REG_LINK_CONTROL            (0x10u)
#define PCIE_CAP_REG_LINK_STATUS             (0x12u)
#define PCIE_LINK_CONTROL_RETRAIN            (0x0020u)
#define PCIE_LINK_STATUS_SPEED_MASK          (0x000Fu)
#define PCIE_LINK_STATUS_SPEED_SHIFT         (0u)
#define PCIE_LINK_STATUS_TRAINING            (1u << 11)
#define PCIE_LINK_STATUS_DLLLA               (1u << 13)

/* PCIe 2.0+ and 6.0 blocks inside the PCIe Capability (byte offsets) */
#define PCIE_OFFS_DEVCAP2                    (0x24u)
#define PCIE_OFFS_DEVCTL2_DEVSTS2            (0x28u) /* lo16/hi16 */
#define PCIE_OFFS_LNKCAP2                    (0x2Cu)
#define PCIE_OFFS_LNKCTL2_LNKSTS2            (0x30u) /* lo16/hi16 */
#define PCIE_OFFS_DEVCAP3                    (0x34u)
#define PCIE_OFFS_DEVCTL3_DEVSTS3            (0x38u) /* lo16/hi16 */
#define PCIE_OFFS_LNKCAP3                    (0x3Cu)
#define PCIE_OFFS_LNKCTL3_LNKSTS3            (0x40u) /* lo16/hi16 */

/* Link Control 2 Target Link Speed */
#define PCIE_LNKCTL2_TARGET_SPEED_MASK       (0x000Fu)
#define PCIE_LNKCTL2_TARGET_SPEED_64G        (0x0006u)

/* PCIe 6.0 Flit mode indicators per spec */
#define PCIE_CAPS_HDR_FLIT_SUPPORTED         (1u << 31) /* Cap header dword bit31 */
#define PCIE_LNKSTS2_FLIT_ACTIVE             (1u << 10) /* In Link Status 2 (upper 16 bits of +0x30) */

/* Device Control/Status 3 bit usage (commonly: enable=bit0; status mirrored in LnkSts2) */
#define PCIE_DEVCTL3_FLIT_ENABLE             (0x0001u)

/* PL64G Extended Capability offsets (relative to PL64G base) */
#define PL64G_OFFS_CAP_HDR                   (0x00u)
#define PL64G_OFFS_CAPABILITY                (0x04u)
#define PL64G_OFFS_CONTROL                   (0x08u)
#define PL64G_OFFS_STATUS                    (0x0Cu)

struct pcie_cfg_ops {
    uint32_t (*cfg_read32)(void *cookie, uint32_t offset);
    void     (*cfg_write32)(void *cookie, uint32_t offset, uint32_t value);
};

struct pcie_flit64_ctx {
    void *cookie;
    const struct pcie_cfg_ops *ops;
    uint16_t pcie_cap_base;
    uint32_t pl64g_base; /* 0 if not present */
};

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
                               bool retrain_link,
                               uint32_t timeout_us);

/* Inline config helpers (compose 16/8 on top of 32) */
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

#ifdef __cplusplus
}
#endif

#endif /* PCIE_FLIT64_CFG_H */
