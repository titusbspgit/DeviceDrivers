/*
 * Synopsys DesignWare PCIe (DM v6.00a) — PCIe 6.0 64.0 GT/s Flit Mode Feature Driver (Source)
 *
 * Implements initialization and enable sequence for FLIT mode at 64.0 GT/s
 * using DBI access to PCIe Capability and Port Logic registers.
 *
 * RCI-ag-DrvGen Agent
 */

#include "dw_pcie6_flit.h"
#include <stdio.h>

/* Internal helpers: strictly 32-bit MMIO */
static inline uint32_t reg_read32(volatile uint8_t *base, uint32_t off)
{
    volatile uint32_t *p = (volatile uint32_t *)(void *)(base + (off & ~0x3u));
    (void)off; /* off used for alignment masking above */
    return *p; /* Assumes little-endian bus; standard for DW DBI */
}

static inline void reg_write32(volatile uint8_t *base, uint32_t off, uint32_t val)
{
    volatile uint32_t *p = (volatile uint32_t *)(void *)(base + (off & ~0x3u));
    (void)off;
    *p = val;
}

static inline uint16_t reg_read16(volatile uint8_t *base, uint32_t off)
{
    uint32_t aligned = off & ~0x3u;
    uint32_t shift = (off & 0x2u) * 8u; /* 0 for low half, 16 for high half */
    uint32_t v = reg_read32(base, aligned);
    return (uint16_t)((v >> shift) & 0xFFFFu);
}

static inline void reg_write16(volatile uint8_t *base, uint32_t off, uint16_t val)
{
    uint32_t aligned = off & ~0x3u;
    uint32_t shift = (off & 0x2u) * 8u;
    uint32_t mask = 0xFFFFu << shift;
    uint32_t v = reg_read32(base, aligned);
    v = (v & ~mask) | (((uint32_t)val << shift) & mask);
    reg_write32(base, aligned, v);
}

static inline uint8_t reg_read8(volatile uint8_t *base, uint32_t off)
{
    uint32_t aligned = off & ~0x3u;
    uint32_t shift = (off & 0x3u) * 8u;
    uint32_t v = reg_read32(base, aligned);
    return (uint8_t)((v >> shift) & 0xFFu);
}

static inline void set_bits16(volatile uint8_t *base, uint32_t off, uint16_t mask)
{
    uint16_t v = reg_read16(base, off);
    v = (uint16_t)(v | mask);
    reg_write16(base, off, v);
}

static inline void clr_bits16(volatile uint8_t *base, uint32_t off, uint16_t mask)
{
    uint16_t v = reg_read16(base, off);
    v = (uint16_t)(v & (uint16_t)~mask);
    reg_write16(base, off, v);
}

/* Delay helper */
static inline void do_udelay(const dw_pcie6_t *ctx, uint32_t usec)
{
    if ((ctx != NULL) && (ctx->udelay != NULL)) {
        ctx->udelay(usec);
    } else {
        /* Busy-wait best-effort: simple volatile loop (platform dependent). */
        volatile uint32_t i;
        for (i = 0u; i < (usec * 50u); ++i) {
            __asm__ __volatile__("" ::: "memory");
        }
    }
}

/* Private: find capability with Cap ID within standard Cap list */
static int find_pcie_capability(dw_pcie6_t *ctx)
{
    if ((ctx == NULL) || (ctx->dbi_base == NULL)) {
        return DW_PCIE6_ERR_PARAM;
    }

    uint8_t cap_ptr = reg_read8(ctx->dbi_base, PCI_CFG_CAP_PTR_OFF);
    /* Traverse single-byte next pointers, 0 indicates end */
    uint32_t guard = 0u;
    while ((cap_ptr != 0u) && (guard < 256u)) {
        uint8_t cap_id = reg_read8(ctx->dbi_base, (uint32_t)cap_ptr + 0u);
        uint8_t next   = reg_read8(ctx->dbi_base, (uint32_t)cap_ptr + 1u);
        if (cap_id == (uint8_t)PCIE_CAP_ID) {
            ctx->pcie_cap_off = (uint32_t)cap_ptr;
            return DW_PCIE6_OK;
        }
        cap_ptr = next;
        guard++;
    }

    ctx->pcie_cap_off = 0u;
    return DW_PCIE6_ERR_CAP_NOT_FOUND;
}

/* Private: find PL64G Extended Capability in Ext Cap list starting at 0x100 */
static int find_pl64g_ext_cap(dw_pcie6_t *ctx)
{
    if ((ctx == NULL) || (ctx->dbi_base == NULL)) {
        return DW_PCIE6_ERR_PARAM;
    }

    uint32_t off = PCIE_EXT_CAP_START;
    uint32_t guard = 0u;
    while ((off != 0u) && (guard < 256u)) {
        uint32_t hdr = reg_read32(ctx->dbi_base, off);
        uint16_t id = (uint16_t)(hdr & (uint32_t)PCIE_EXT_CAP_HDR_ID_MASK);
        uint32_t next = (hdr & (uint32_t)PCIE_EXT_CAP_HDR_NEXT_MASK) >> PCIE_EXT_CAP_HDR_NEXT_SHIFT;
        if (id == (uint16_t)PCIE_EXT_CAP_ID_PL64G) {
            ctx->pl64g_cap_off = off;
            return DW_PCIE6_OK;
        }
        if (next == 0u) {
            break;
        }
        off = next;
        guard++;
    }

    ctx->pl64g_cap_off = 0u;
    return DW_PCIE6_ERR_CAP_NOT_FOUND;
}

int dw_pcie6_init(dw_pcie6_t *ctx, uintptr_t dbi_base_addr)
{
    if ((ctx == NULL) || (dbi_base_addr == (uintptr_t)0)) {
        return DW_PCIE6_ERR_PARAM;
    }
    ctx->dbi_base = (volatile uint8_t *)(void *)dbi_base_addr;
    ctx->pcie_cap_off = 0u;
    ctx->pl64g_cap_off = 0u;
    ctx->udelay = NULL;

    /* Probe capabilities now for convenience */
    (void)find_pcie_capability(ctx);
    (void)find_pl64g_ext_cap(ctx);

    return DW_PCIE6_OK;
}

void dw_pcie6_set_udelay(dw_pcie6_t *ctx, void (*udelay_cb)(uint32_t))
{
    if (ctx != NULL) {
        ctx->udelay = udelay_cb;
    }
}

int dw_pcie6_find_caps(dw_pcie6_t *ctx)
{
    int rc;
    rc = find_pcie_capability(ctx);
    if (rc != DW_PCIE6_OK) {
        return rc;
    }
    rc = find_pl64g_ext_cap(ctx);
    (void)rc; /* PL64G presence optional; not required for programming sequence */
    return DW_PCIE6_OK;
}

bool dw_pcie6_is_flit_supported(const dw_pcie6_t *ctx)
{
    if ((ctx == NULL) || (ctx->dbi_base == NULL) || (ctx->pcie_cap_off == 0u)) {
        return false;
    }
    uint32_t devcap3 = reg_read32(ctx->dbi_base, ctx->pcie_cap_off + PCIE_CAP_DEVCAP3_OFF);
    return ((devcap3 & (uint32_t)PCIE_DEVCAP3_FLIT_MODE_SUPP) != 0u);
}

int dw_pcie6_get_status(dw_pcie6_t *ctx, bool *flit_mode, uint8_t *cur_speed, bool *dl_active)
{
    if ((ctx == NULL) || (ctx->dbi_base == NULL) || (ctx->pcie_cap_off == 0u)) {
        return DW_PCIE6_ERR_PARAM;
    }

    uint16_t lnksta = reg_read16(ctx->dbi_base, ctx->pcie_cap_off + PCIE_CAP_LNKCTLSTS_OFF + 2u);
    uint32_t devctl3sts3 = reg_read32(ctx->dbi_base, ctx->pcie_cap_off + PCIE_CAP_DEVCTL3STS3_OFF);
    uint16_t devsts3 = (uint16_t)((devctl3sts3 >> 16) & 0xFFFFu);

    if (flit_mode != NULL) {
        *flit_mode = ((devsts3 & (uint16_t)PCIE_DEVSTS3_FLIT_MODE_STATUS) != 0u);
    }
    if (cur_speed != NULL) {
        *cur_speed = (uint8_t)(lnksta & (uint16_t)PCIE_LNKSTA_CLS_MASK);
    }
    if (dl_active != NULL) {
        *dl_active = ((lnksta & (uint16_t)PCIE_LNKSTA_DLLLA) != 0u);
    }

    return DW_PCIE6_OK;
}

int dw_pcie6_enable_flit_64g(dw_pcie6_t *ctx, uint32_t timeout_us)
{
    if ((ctx == NULL) || (ctx->dbi_base == NULL)) {
        return DW_PCIE6_ERR_PARAM;
    }

    /* Ensure PCIe Capability is located */
    if (ctx->pcie_cap_off == 0u) {
        int rc = find_pcie_capability(ctx);
        if (rc != DW_PCIE6_OK) {
            return rc;
        }
    }

    /* Check that FLIT is supported */
    if (!dw_pcie6_is_flit_supported(ctx)) {
        return DW_PCIE6_ERR_UNSUPPORTED;
    }

    /* Enable DBI RO write if needed for LC2/DC3 programming */
    uint32_t misc = reg_read32(ctx->dbi_base, PL_MISC_CONTROL_1_OFF);
    reg_write32(ctx->dbi_base, PL_MISC_CONTROL_1_OFF, misc | (uint32_t)PL_MISC_CONTROL_1_DBI_RO_WR_EN);

    /* Program Target Link Speed = 64.0 GT/s (Gen6) */
    uint16_t lnkctl2 = reg_read16(ctx->dbi_base, ctx->pcie_cap_off + PCIE_CAP_LNKCTL2STS2_OFF);
    lnkctl2 = (uint16_t)((lnkctl2 & (uint16_t)~PCIE_LNKCTL2_TLS_MASK) | (uint16_t)PCIE_LINK_SPEED_GEN6);
    reg_write16(ctx->dbi_base, ctx->pcie_cap_off + PCIE_CAP_LNKCTL2STS2_OFF, lnkctl2);

    /* Request Enter FLIT Mode */
    uint16_t devctl3 = reg_read16(ctx->dbi_base, ctx->pcie_cap_off + PCIE_CAP_DEVCTL3STS3_OFF);
    devctl3 = (uint16_t)(devctl3 | (uint16_t)PCIE_DEVCTL3_ENTER_FLIT_MODE);
    reg_write16(ctx->dbi_base, ctx->pcie_cap_off + PCIE_CAP_DEVCTL3STS3_OFF, devctl3);

    /* Retrain link */
    set_bits16(ctx->dbi_base, ctx->pcie_cap_off + PCIE_CAP_LNKCTLSTS_OFF, (uint16_t)PCIE_LNKCTL_RL);

    /* Poll for: Data Link Active, Current Link Speed == Gen6, FLIT Mode Status == 1 */
    uint32_t waited = 0u;
    while (waited < timeout_us) {
        bool flit = false;
        uint8_t speed = 0u;
        bool dll = false;
        (void)dw_pcie6_get_status(ctx, &flit, &speed, &dll);
        if (flit && dll && (speed == (uint8_t)PCIE_LINK_SPEED_GEN6)) {
            break;
        }
        do_udelay(ctx, 10u);
        waited += 10u;
    }

    /* Lock DBI RO back */
    misc = reg_read32(ctx->dbi_base, PL_MISC_CONTROL_1_OFF);
    reg_write32(ctx->dbi_base, PL_MISC_CONTROL_1_OFF, misc & (uint32_t)~PL_MISC_CONTROL_1_DBI_RO_WR_EN);

    if (waited >= timeout_us) {
        return DW_PCIE6_ERR_TIMEOUT;
    }

    return DW_PCIE6_OK;
}
