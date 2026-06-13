/*
 * Synopsys DesignWare PCIe (DM) v6.00a - ASPM L0s/L1 Control
 * Source
 */
#include "dw_pcie_dm_aspm.h"

#ifndef DW_PCIE_DM_BIG_ENDIAN
#define CPU_TO_LE32(x) ((uint32_t)(x))
#define LE32_TO_CPU(x) ((uint32_t)(x))
#else
#define CPU_TO_LE32(x) (__builtin_bswap32((uint32_t)(x)))
#define LE32_TO_CPU(x) (__builtin_bswap32((uint32_t)(x)))
#endif

static inline uint32_t mmio_read32(const volatile void *addr)
{
    const volatile uint32_t *p = (const volatile uint32_t *)addr;
    return LE32_TO_CPU(*p);
}

static inline void mmio_write32(volatile void *addr, uint32_t value)
{
    volatile uint32_t *p = (volatile uint32_t *)addr;
    *p = CPU_TO_LE32(value);
    (void)*p;
}

static uint8_t cfg_read8(const dw_pcie_dm_aspm_ctx_t *ctx, uint32_t off)
{
    const volatile uint8_t *p = (const volatile uint8_t *)(ctx->dbi_base + off);
    return *p;
}

static uint16_t cfg_read16(const dw_pcie_dm_aspm_ctx_t *ctx, uint32_t off)
{
    const uint32_t w = mmio_read32((const void *)(ctx->dbi_base + (off & ~0x3u)));
    return (uint16_t)(w >> ((off & 0x2u) * 8u));
}

static void cfg_write16(const dw_pcie_dm_aspm_ctx_t *ctx, uint32_t off, uint16_t val)
{
    const uint32_t a = (off & ~0x3u);
    const uint32_t s = (off & 0x2u) * 8u;
    uint32_t w = mmio_read32((const void *)(ctx->dbi_base + a));
    w &= ~(0xFFFFu << s);
    w |= ((uint32_t)val << s);
    mmio_write32((void *)(ctx->dbi_base + a), w);
}

static uint32_t find_pcie_cap(const dw_pcie_dm_aspm_ctx_t *ctx)
{
    uint8_t ptr = cfg_read8(ctx, PCIE_CAP_OFF_PTR);
    for (uint32_t guard = 0u; (ptr != 0u) && (guard < 48u); ++guard) {
        const uint16_t id = cfg_read16(ctx, (uint32_t)ptr + 0x0u);
        if ((id & 0xFFu) == PCIE_CAP_ID_PCI_EXPRESS) {
            return (uint32_t)ptr;
        }
        ptr = (uint8_t)(cfg_read16(ctx, (uint32_t)ptr + 0x2u) & 0xFFu);
    }
    return 0u;
}

int32_t dw_pcie_dm_aspm_init(dw_pcie_dm_aspm_ctx_t *ctx, uintptr_t dbi_base, uintptr_t dsp_base)
{
    if ((ctx == (void *)0) || (dbi_base == (uintptr_t)0)) { return DW_PCIE_DM_EINVAL; }
    ctx->dbi_base = (volatile uint8_t *)dbi_base;
    ctx->dsp_base = (volatile uint8_t *)dsp_base; /* may be NULL (0) */
    return DW_PCIE_DM_OK;
}

int32_t dw_pcie_dm_aspm_set_mode(const dw_pcie_dm_aspm_ctx_t *ctx, dw_pcie_aspm_mode_t mode)
{
    if (ctx == (void *)0) { return DW_PCIE_DM_EINVAL; }
    const uint32_t cap = find_pcie_cap(ctx);
    if (cap == 0u) { return DW_PCIE_DM_ENOTFOUND; }
    uint16_t lc = cfg_read16(ctx, cap + PCIE_CAP_OFF_LINK_CONTROL);
    lc &= ~(uint16_t)0x3u; /* clear ASPM control */
    lc |= (uint16_t)(mode & 0x3u);
    cfg_write16(ctx, cap + PCIE_CAP_OFF_LINK_CONTROL, lc);
    return DW_PCIE_DM_OK;
}

int32_t dw_pcie_dm_aspm_get_mode(const dw_pcie_dm_aspm_ctx_t *ctx, dw_pcie_aspm_mode_t *mode)
{
    if ((ctx == (void *)0) || (mode == (void *)0)) { return DW_PCIE_DM_EINVAL; }
    const uint32_t cap = find_pcie_cap(ctx);
    if (cap == 0u) { return DW_PCIE_DM_ENOTFOUND; }
    const uint16_t lc = cfg_read16(ctx, cap + PCIE_CAP_OFF_LINK_CONTROL);
    *mode = (dw_pcie_aspm_mode_t)(lc & 0x3u);
    return DW_PCIE_DM_OK;
}

int32_t dw_pcie_dm_aspm_vendor_rmw(const dw_pcie_dm_aspm_ctx_t *ctx, uint32_t reg_off, uint32_t mask, uint32_t value)
{
    if ((ctx == (void *)0) || (ctx->dsp_base == (void *)0) || (mask == 0u) || ((value & ~mask) != 0u)) {
        return DW_PCIE_DM_EINVAL;
    }
    const volatile void *addr = (const void *)(ctx->dsp_base + reg_off);
    const uint32_t cur = mmio_read32(addr);
    const uint32_t newv = (cur & ~mask) | (value & mask);
    mmio_write32((void *)addr, newv);
    const uint32_t rb = mmio_read32(addr);
    return (rb == newv) ? DW_PCIE_DM_OK : DW_PCIE_DM_EBUSY;
}
