/*
 * SPDX-License-Identifier: MIT
 * Common HAL and utilities for Synopsys DesignWare PCIe (DM v6.00a) - ARM big-endian
 */
#ifndef PCIE_HAL_H
#define PCIE_HAL_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef PCIE_HAL_HOST_TEST
#define PCIE_HAL_HOST_TEST (0)
#endif

#if defined(__BYTE_ORDER__) && defined(__ORDER_BIG_ENDIAN__)
# if (__BYTE_ORDER__ != __ORDER_BIG_ENDIAN__)
#  if (PCIE_HAL_HOST_TEST)
#  else
#   warning "Building for non-big-endian target; MMIO helpers will byte-swap if needed."
#  endif
# endif
#endif

typedef enum
{
    PCIE_STATUS_OK = 0,
    PCIE_STATUS_ERROR = -1,
    PCIE_STATUS_TIMEOUT = -2,
    PCIE_STATUS_UNSUPPORTED = -3,
    PCIE_STATUS_BUSY = -4,
    PCIE_STATUS_INVALID_ARG = -5
} pcie_status_t;

typedef struct
{
    volatile uint8_t* dbi;
    size_t            dbi_size;
} pcie_dbi_t;

void pcie_hal_udelay(uint32_t usec);

static inline void pcie_wmb(void) { __asm__ volatile ("" ::: "memory"); }
static inline void pcie_rmb(void) { __asm__ volatile ("" ::: "memory"); }

static inline uint32_t pcie_read32(const pcie_dbi_t* ctx, uint32_t off)
{
    volatile uint32_t* p = (volatile uint32_t*)(ctx->dbi + off);
#if (PCIE_HAL_HOST_TEST)
    return *p;
#else
# if defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
    uint32_t v = *p; v = __builtin_bswap32(v); return v;
# else
    return *p;
# endif
#endif
}

static inline void pcie_write32(pcie_dbi_t* ctx, uint32_t off, uint32_t val)
{
    volatile uint32_t* p = (volatile uint32_t*)(ctx->dbi + off);
#if (PCIE_HAL_HOST_TEST)
    *p = val;
#else
# if defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
    *p = __builtin_bswap32(val);
# else
    *p = val;
# endif
#endif
    pcie_wmb();
}

static inline void pcie_rmw32(pcie_dbi_t* ctx, uint32_t off, uint32_t clr_mask, uint32_t set_mask)
{
    uint32_t v = pcie_read32(ctx, off);
    v = (v & (~clr_mask)) | set_mask;
    pcie_write32(ctx, off, v);
}

static inline pcie_status_t pcie_init(pcie_dbi_t* ctx, volatile void* dbi_base, size_t dbi_size)
{
    if ((ctx == NULL) || (dbi_base == NULL) || (dbi_size < 0x100u)) {
        return PCIE_STATUS_INVALID_ARG;
    }
    ctx->dbi = (volatile uint8_t*)dbi_base;
    ctx->dbi_size = dbi_size;
    return PCIE_STATUS_OK;
}

#define PCI_CFG_CAP_PTR_OFF                 (0x34u)
#define PCI_CAP_ID_PCI_EXP                  (0x10u)

static inline uint8_t pcie_read8(const pcie_dbi_t* ctx, uint32_t off)
{
    volatile uint8_t* p = (volatile uint8_t*)(ctx->dbi + off);
    return *p;
}

static inline void pcie_write8(pcie_dbi_t* ctx, uint32_t off, uint8_t v)
{
    volatile uint8_t* p = (volatile uint8_t*)(ctx->dbi + off);
    *p = v; pcie_wmb();
}

static inline uint16_t pcie_read16(const pcie_dbi_t* ctx, uint32_t off)
{
    volatile uint16_t* p = (volatile uint16_t*)(ctx->dbi + off);
#if (PCIE_HAL_HOST_TEST)
    return *p;
#else
# if defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
    uint16_t v = *p; v = __builtin_bswap16(v); return v;
# else
    return *p;
# endif
#endif
}

static inline void pcie_write16(pcie_dbi_t* ctx, uint32_t off, uint16_t v)
{
    volatile uint16_t* p = (volatile uint16_t*)(ctx->dbi + off);
#if (PCIE_HAL_HOST_TEST)
    *p = v;
#else
# if defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
    *p = __builtin_bswap16(v);
# else
    *p = v;
# endif
#endif
    pcie_wmb();
}

static inline uint32_t pcie_find_pci_cap(const pcie_dbi_t* ctx, uint8_t cap_id)
{
    uint8_t ptr = pcie_read8(ctx, PCI_CFG_CAP_PTR_OFF) & (uint8_t)~0x3u;
    uint32_t limit = 0u;
    while ((ptr != 0u) && (limit < 48u)) {
        uint8_t id = pcie_read8(ctx, (uint32_t)ptr);
        if (id == cap_id) { return (uint32_t)ptr; }
        ptr = pcie_read8(ctx, (uint32_t)ptr + 1u) & (uint8_t)~0x3u;
        limit++;
    }
    return 0u;
}

static inline uint32_t pcie_find_ext_cap(const pcie_dbi_t* ctx, uint16_t ext_cap_id)
{
    uint32_t off = 0x100u; uint32_t limit = 0u;
    while ((off != 0u) && (off < ctx->dbi_size) && (limit < 1024u)) {
        uint32_t hdr = pcie_read32(ctx, off);
        if ((hdr & 0xFFFFu) == (uint32_t)ext_cap_id) { return off; }
        uint32_t next = (hdr >> 20) & 0xFFFu; if (next == 0u) { break; }
        off = next; limit++;
    }
    return 0u;
}

#define PCIE_CAP_REG_PXCAP           (0x02u)
#define PCIE_CAP_REG_DEV_CAP2        (0x24u)
#define PCIE_CAP_REG_DEV_CTRL2_STAT2 (0x28u)
#define PCIE_CAP_REG_LINK_CAP2       (0x2Cu)
#define PCIE_CAP_REG_LINK_CTRL2_STAT2 (0x30u)
#define PCIE_CAP_REG_DEV_CAP3        (0x38u)
#define PCIE_CAP_REG_LINK_CTRL3_STAT3 (0x3Cu)

#define DEV_CAP3_FLIT_MODE_SUPP      (1u << 0)

#define PCIE_EXTCAP_ID_L1SUB         (0x001Eu)
#define PCIE_EXTCAP_ID_SPCIE         (0x0019u)
#define PCIE_EXTCAP_ID_PL64G         (0x0031u)

#define DWC_PORT_VSDLLP_OFF          (0x704u)
#define DWC_PORT_FORCE_OFF           (0x708u)
#define DWC_GEN3_RELATED_OFF         (0x890u)
#define DWC_GEN3_EQ_LOCAL_FS_LF_OFF  (0x894u)
#define DWC_GEN3_EQ_PSET_COEF_MAP_OFF (0x898u)
#define DWC_GEN3_EQ_PRESET_INDEX_OFF  (0x89Cu)
#define DWC_GEN3_EQ_STATUS_OFF        (0x8A4u)

#define DWC_PHY_VIEWPORT_CTLSTS_OFF  (0xB70u)
#define DWC_PHY_VIEWPORT_DATA_OFF    (0xB74u)

#ifdef __cplusplus
}
#endif

#endif /* PCIE_HAL_H */
