/*
 * Synopsys DesignWare PCIe (DWC) – Bare-metal driver source
 * Controller: DWHSynopsis (DesignWare PCIe Embedded Host Controller)
 * Target: ARM (big-endian CPU), Bare metal, GCC
 * Author: RCI-ag-DrvGen Agent
 */

#include "../include/dwc_pcie_dwhsynopsis.h"
#include <stdio.h>

/* Weak platform hooks default implementations */
__attribute__((weak)) void dwc_pcie_platform_ltssm_enable(const dwc_pcie_dev_t *dev)
{
    (void)dev; /* Platform should gate LTSSM via SoC-specific control if required */
}

__attribute__((weak)) void dwc_udelay(uint32_t usec)
{
    volatile uint32_t i;
    /* Crude delay loop: platform should override for accuracy */
    for (i = 0u; i < (usec * 16u); ++i) {
        __asm__ __volatile__("" ::: "memory");
    }
}

static void dwc_pcie_setbits(uintptr_t base, uint32_t off, uint32_t mask)
{
    uint32_t v = dwc_pcie_mmio_read32(base, off);
    v |= mask;
    dwc_pcie_mmio_write32(base, off, v);
}

static void dwc_pcie_clrbits(uintptr_t base, uint32_t off, uint32_t mask)
{
    uint32_t v = dwc_pcie_mmio_read32(base, off);
    v &= ~mask;
    dwc_pcie_mmio_write32(base, off, v);
}

void dwc_pcie_init(dwc_pcie_dev_t *dev, uintptr_t dbi_base, uint32_t timeout_ms,
                   uint32_t pcie_cap_off, uint32_t spcie_cap_off, uint32_t pl64g_cap_off)
{
    if (dev == NULL) {
        return;
    }
    dev->dbi_base    = dbi_base;
    dev->timeout_ms  = timeout_ms;
    dev->pcie_cap_off = pcie_cap_off;
    dev->spcie_cap_off = spcie_cap_off;
    dev->pl64g_cap_off = pl64g_cap_off;
}

void dwc_pcie_dbi_ro_write_enable(dwc_pcie_dev_t *dev, bool enable)
{
    if (dev == NULL) { return; }
    if (enable) {
        dwc_pcie_setbits(dev->dbi_base, DWC_PCIE_PL_MISC_CONTROL_1_OFF,
                         DWC_PCIE_PL_MISC_CONTROL_1_DBI_RO_WR_EN_MASK);
    } else {
        dwc_pcie_clrbits(dev->dbi_base, DWC_PCIE_PL_MISC_CONTROL_1_OFF,
                         DWC_PCIE_PL_MISC_CONTROL_1_DBI_RO_WR_EN_MASK);
    }
}

static uint8_t dwc_pcie_cfg_read8(const dwc_pcie_dev_t *dev, uint32_t off)
{
    /* DBI config space is accessible at DBI base + off; use 32-bit aligned access */
    uint32_t aligned = off & ~0x3u;
    uint32_t shift = (off & 0x3u) * 8u;
    uint32_t v = dwc_pcie_mmio_read32(dev->dbi_base, aligned);
    return (uint8_t)((v >> shift) & 0xFFu);
}

static void dwc_pcie_cfg_write8(const dwc_pcie_dev_t *dev, uint32_t off, uint8_t val)
{
    uint32_t aligned = off & ~0x3u;
    uint32_t shift = (off & 0x3u) * 8u;
    uint32_t mask = 0xFFu << shift;
    uint32_t v = dwc_pcie_mmio_read32(dev->dbi_base, aligned);
    v = (v & ~mask) | (((uint32_t)val << shift) & mask);
    dwc_pcie_mmio_write32(dev->dbi_base, aligned, v);
}

static uint16_t dwc_pcie_cfg_read16(const dwc_pcie_dev_t *dev, uint32_t off)
{
    uint32_t aligned = off & ~0x3u;
    uint32_t shift = (off & 0x2u) * 8u; /* 0 or 16 */
    uint32_t v = dwc_pcie_mmio_read32(dev->dbi_base, aligned);
    return (uint16_t)((v >> shift) & 0xFFFFu);
}

static void dwc_pcie_cfg_write16(const dwc_pcie_dev_t *dev, uint32_t off, uint16_t val)
{
    uint32_t aligned = off & ~0x3u;
    uint32_t shift = (off & 0x2u) * 8u;
    uint32_t mask = 0xFFFFu << shift;
    uint32_t v = dwc_pcie_mmio_read32(dev->dbi_base, aligned);
    v = (v & ~mask) | (((uint32_t)val << shift) & mask);
    dwc_pcie_mmio_write32(dev->dbi_base, aligned, v);
}

static uint32_t dwc_pcie_cfg_read32(const dwc_pcie_dev_t *dev, uint32_t off)
{
    return dwc_pcie_mmio_read32(dev->dbi_base, off);
}

static void dwc_pcie_cfg_write32(const dwc_pcie_dev_t *dev, uint32_t off, uint32_t val)
{
    dwc_pcie_mmio_write32(dev->dbi_base, off, val);
}

/* Discover PCIe Capability base by walking standard capability list */
dwc_pcie_status_t dwc_pcie_find_pcie_cap(dwc_pcie_dev_t *dev, uint32_t *cap_off)
{
    if ((dev == NULL) || (cap_off == NULL)) { return DWC_PCIE_E_PARAM; }

    uint16_t status = dwc_pcie_cfg_read16(dev, DWC_PCIE_PCI_STATUS_OFF);
    if ((status & (uint16_t)DWC_PCIE_PCI_STATUS_CAP_LIST_MASK) == 0u) {
        return DWC_PCIE_E_UNSUPPORTED;
    }

    uint8_t ptr = dwc_pcie_cfg_read8(dev, DWC_PCIE_PCI_CAP_PTR_OFF);
    /* Cap pointers are 4-byte aligned and terminate at 0 */
    for (uint32_t guard = 0u; (ptr != 0u) && (guard < 256u); ++guard) {
        uint8_t id = dwc_pcie_cfg_read8(dev, (uint32_t)ptr);
        if (id == (uint8_t)DWC_PCIE_CAP_ID_PCIE) {
            dev->pcie_cap_off = (uint32_t)ptr;
            *cap_off = (uint32_t)ptr;
            return DWC_PCIE_OK;
        }
        ptr = dwc_pcie_cfg_read8(dev, (uint32_t)ptr + 1u);
    }
    return DWC_PCIE_E_UNSUPPORTED;
}

/* Find PCIe Extended Capability by ID (searches from 0x100). Returns 0 if not found */
uint32_t dwc_pcie_find_ext_cap(const dwc_pcie_dev_t *dev, uint16_t cap_id)
{
    if (dev == NULL) { return 0u; }
    uint32_t off = 0x100u; /* Ext caps start here */
    for (uint32_t guard = 0u; guard < 1024u; ++guard) {
        uint32_t hdr = dwc_pcie_cfg_read32(dev, off);
        if (hdr == 0u) { break; }
        uint16_t id = (uint16_t)(hdr & 0xFFFFu);
        uint16_t next = (uint16_t)((hdr >> 20) & 0xFFFu);
        if (id == cap_id) {
            return off;
        }
        if (next == 0u) { break; }
        off = (uint32_t)next;
    }
    return 0u;
}

static bool dwc_pcie_poll_mask(uintptr_t base, uint32_t off, uint32_t mask, bool set,
                               uint32_t timeout_ms)
{
    const uint32_t poll_us = 50u;
    uint32_t waited = 0u;
    for (;;) {
        uint32_t v = dwc_pcie_mmio_read32(base, off);
        bool cond = ((v & mask) != 0u);
        if ((set && cond) || (!set && !cond)) {
            return true;
        }
        if (waited >= (timeout_ms * 1000u)) { break; }
        dwc_udelay(poll_us);
        waited += poll_us;
    }
    return false;
}

/* PHY ready polling */
dwc_pcie_status_t dwc_pcie_wait_phy_ready(const dwc_pcie_dev_t *dev, uint32_t timeout_ms)
{
    if (dev == NULL) { return DWC_PCIE_E_PARAM; }
    bool ok = dwc_pcie_poll_mask(dev->dbi_base, DWC_PCIE_PL_PHY_STATUS_OFF,
                                 DWC_PCIE_PL_PHY_STATUS_PHY_RDY_MASK, true, timeout_ms);
    return ok ? DWC_PCIE_OK : DWC_PCIE_E_TIMEOUT;
}

/* Program target link speed in Link Control 2 (does not trigger training by itself) */
dwc_pcie_status_t dwc_pcie_set_target_link_speed(const dwc_pcie_dev_t *dev, uint8_t speed_code)
{
    if (dev == NULL) { return DWC_PCIE_E_PARAM; }
    uint32_t cap_off = dev->pcie_cap_off;
    if (cap_off == 0u) {
        if (dwc_pcie_find_pcie_cap((dwc_pcie_dev_t *)dev, &cap_off) != DWC_PCIE_OK) {
            return DWC_PCIE_E_UNSUPPORTED;
        }
    }
    uint16_t lnkctl2 = dwc_pcie_cfg_read16(dev, cap_off + DWC_PCIE_CAP_LNKCTL2_OFF);
    lnkctl2 = (uint16_t)((lnkctl2 & (uint16_t)~0xFu) | (uint16_t)(speed_code & 0xFu));
    dwc_pcie_cfg_write16(dev, cap_off + DWC_PCIE_CAP_LNKCTL2_OFF, lnkctl2);
    return DWC_PCIE_OK;
}

/* Start link training via platform LTSSM enable and optional directed speed change */
dwc_pcie_status_t dwc_pcie_start_link_training(const dwc_pcie_dev_t *dev)
{
    if (dev == NULL) { return DWC_PCIE_E_PARAM; }
    /* Some platforms require a directed speed change kick */
    dwc_pcie_setbits(dev->dbi_base, DWC_PCIE_PL_GEN2_CTRL_OFF,
                     DWC_PCIE_PL_GEN2_CTRL_DIR_SPEED_CHANGE_MASK);
    dwc_pcie_platform_ltssm_enable(dev);
    return DWC_PCIE_OK;
}

/* Wait for link up: consider link up when Negotiated Link Width != 0 */
dwc_pcie_status_t dwc_pcie_wait_link_up(const dwc_pcie_dev_t *dev, uint32_t timeout_ms)
{
    if (dev == NULL) { return DWC_PCIE_E_PARAM; }
    uint32_t cap_off = dev->pcie_cap_off;
    if (cap_off == 0u) {
        if (dwc_pcie_find_pcie_cap((dwc_pcie_dev_t *)dev, &cap_off) != DWC_PCIE_OK) {
            return DWC_PCIE_E_UNSUPPORTED;
        }
    }
    const uint32_t poll_us = 1000u;
    uint32_t waited = 0u;
    for (;;) {
        uint16_t lnksta = dwc_pcie_cfg_read16(dev, cap_off + DWC_PCIE_CAP_LNKSTA_OFF);
        if ((lnksta & (uint16_t)DWC_PCIE_LNKSTA_NLW_MASK) != 0u) {
            return DWC_PCIE_OK;
        }
        if (waited >= (timeout_ms * 1000u)) { break; }
        dwc_udelay(poll_us);
        waited += poll_us;
    }
    return DWC_PCIE_E_TIMEOUT;
}

/* iATU: program an outbound region (viewport model) */
dwc_pcie_status_t dwc_pcie_iatu_outbound_set(const dwc_pcie_dev_t *dev,
    uint32_t region_index,
    uint32_t tlp_type,
    uint64_t cpu_base,
    uint64_t size,
    uint64_t pci_target,
    uint32_t cr1_attrs,
    uint32_t cr2_attrs)
{
    if ((dev == NULL) || (size == 0u)) { return DWC_PCIE_E_PARAM; }

    /* Alignment: require base aligned to size, size power-of-two, min 4KB */
    if (((size & (size - 1u)) != 0u) || ((size & 0xFFFu) != 0u)) {
        return DWC_PCIE_E_PARAM;
    }
    if (((cpu_base & (size - 1u)) != 0u) || ((pci_target & (size - 1u)) != 0u)) {
        return DWC_PCIE_E_PARAM;
    }

    /* Select viewport */
    uint32_t vp = DWC_PCIE_IATU_VIEWPORT_DIR_OUTBOUND | (region_index & DWC_PCIE_IATU_VIEWPORT_REGION_INDEX_MASK);
    dwc_pcie_mmio_write32(dev->dbi_base, DWC_PCIE_IATU_VIEWPORT_OFF, vp);

    /* Program address window (limit is inclusive) */
    uint32_t lwr_base = (uint32_t)(cpu_base & 0xFFFFFFFFu);
    uint32_t upr_base = (uint32_t)((cpu_base >> 32) & 0xFFFFFFFFu);
    uint64_t limit = cpu_base + (size - 1u);
    uint32_t lwr_lim  = (uint32_t)(limit & 0xFFFFFFFFu);

    uint32_t lwr_tgt  = (uint32_t)(pci_target & 0xFFFFFFFFu);
    uint32_t upr_tgt  = (uint32_t)((pci_target >> 32) & 0xFFFFFFFFu);

    dwc_pcie_mmio_write32(dev->dbi_base, DWC_PCIE_IATU_LWR_BASE_ADDR_OFF, lwr_base);
    dwc_pcie_mmio_write32(dev->dbi_base, DWC_PCIE_IATU_UPPER_BASE_ADDR_OFF, upr_base);
    dwc_pcie_mmio_write32(dev->dbi_base, DWC_PCIE_IATU_LIMIT_ADDR_OFF, lwr_lim);
    dwc_pcie_mmio_write32(dev->dbi_base, DWC_PCIE_IATU_LWR_TARGET_ADDR_OFF, lwr_tgt);
    dwc_pcie_mmio_write32(dev->dbi_base, DWC_PCIE_IATU_UPPER_TARGET_ADDR_OFF, upr_tgt);

    /* Attributes and type */
    uint32_t cr1 = (tlp_type & 0x1Fu) | cr1_attrs;
    dwc_pcie_mmio_write32(dev->dbi_base, DWC_PCIE_IATU_REGION_CTRL1_OFF, cr1);

    /* Enable and any CR2 attrs */
    uint32_t cr2 = (cr2_attrs & ~(DWC_PCIE_IATU_CR2_REGION_EN_MASK)) | DWC_PCIE_IATU_CR2_REGION_EN_MASK;
    dwc_pcie_mmio_write32(dev->dbi_base, DWC_PCIE_IATU_REGION_CTRL2_OFF, cr2);

    /* Poll active */
    bool ok = dwc_pcie_poll_mask(dev->dbi_base, DWC_PCIE_IATU_REGION_CTRL2_OFF,
                                 DWC_PCIE_IATU_CR2_REGION_ACTIVE_MASK, true, dev->timeout_ms);
    return ok ? DWC_PCIE_OK : DWC_PCIE_E_TIMEOUT;
}

/* Disable an outbound region */
dwc_pcie_status_t dwc_pcie_iatu_outbound_disable(const dwc_pcie_dev_t *dev, uint32_t region_index)
{
    if (dev == NULL) { return DWC_PCIE_E_PARAM; }
    uint32_t vp = DWC_PCIE_IATU_VIEWPORT_DIR_OUTBOUND | (region_index & DWC_PCIE_IATU_VIEWPORT_REGION_INDEX_MASK);
    dwc_pcie_mmio_write32(dev->dbi_base, DWC_PCIE_IATU_VIEWPORT_OFF, vp);

    uint32_t cr2 = dwc_pcie_mmio_read32(dev->dbi_base, DWC_PCIE_IATU_REGION_CTRL2_OFF);
    cr2 &= ~DWC_PCIE_IATU_CR2_REGION_EN_MASK;
    dwc_pcie_mmio_write32(dev->dbi_base, DWC_PCIE_IATU_REGION_CTRL2_OFF, cr2);

    bool ok = dwc_pcie_poll_mask(dev->dbi_base, DWC_PCIE_IATU_REGION_CTRL2_OFF,
                                 DWC_PCIE_IATU_CR2_REGION_ACTIVE_MASK, false, dev->timeout_ms);
    return ok ? DWC_PCIE_OK : DWC_PCIE_E_TIMEOUT;
}

/* ASPM enable/disable via PCIe Capability LNKCTL and DWC ACK_F_ASPM_CTRL (advisory) */
dwc_pcie_status_t dwc_pcie_aspm_set(const dwc_pcie_dev_t *dev, bool l0s_en, bool l1_en)
{
    if (dev == NULL) { return DWC_PCIE_E_PARAM; }
    uint32_t cap_off = dev->pcie_cap_off;
    if (cap_off == 0u) {
        if (dwc_pcie_find_pcie_cap((dwc_pcie_dev_t *)dev, &cap_off) != DWC_PCIE_OK) {
            return DWC_PCIE_E_UNSUPPORTED;
        }
    }
    /* Link Control at cap_off + 0x10, bits 0:L0s, 1:L1 */
    uint16_t lnkctl = dwc_pcie_cfg_read16(dev, cap_off + DWC_PCIE_CAP_LNKCTL_LNKSTA_OFF);
    if (l0s_en) { lnkctl |=  (uint16_t)0x0001u; } else { lnkctl &= (uint16_t)~0x0001u; }
    if (l1_en)  { lnkctl |=  (uint16_t)0x0002u; } else { lnkctl &= (uint16_t)~0x0002u; }
    dwc_pcie_cfg_write16(dev, cap_off + DWC_PCIE_CAP_LNKCTL_LNKSTA_OFF, lnkctl);

    /* Advisory DWC control: ACK/ASPM tuning (implementation-specific, keep minimal) */
    uint32_t ack_aspm = dwc_pcie_mmio_read32(dev->dbi_base, DWC_PCIE_PL_ACK_F_ASPM_CTRL_OFF);
    (void)ack_aspm; /* no-op, placeholder for platform policy knobs */
    return DWC_PCIE_OK;
}

/* Basic EQ local coefficient programming (for bring-up only; values SoC-tuned) */
void dwc_pcie_gen3_eq_set_local(const dwc_pcie_dev_t *dev, uint8_t fs, uint8_t lf)
{
    if (dev == NULL) { return; }
    /* Offsets per IP documentation (may vary) */
    const uint32_t GEN3_RELATED_OFF = 0x890u;
    const uint32_t GEN3_EQ_LOCAL_FS_LF_OFF = 0x894u;

    uint32_t v = ((uint32_t)fs & 0xFFu) | (((uint32_t)lf & 0xFFu) << 8);
    dwc_pcie_mmio_write32(dev->dbi_base, GEN3_EQ_LOCAL_FS_LF_OFF, v);
    (void)dwc_pcie_mmio_read32(dev->dbi_base, GEN3_RELATED_OFF); /* read to serialize if needed */
}

/* Flit mode control if PL64G capability is present. enable=true to request Flit Mode. */
dwc_pcie_status_t dwc_pcie_flit_mode_enable(const dwc_pcie_dev_t *dev, bool enable)
{
    if (dev == NULL) { return DWC_PCIE_E_PARAM; }
    if (dev->pl64g_cap_off == 0u) { return DWC_PCIE_E_UNSUPPORTED; }

    uint32_t ctrl_off = dwc_pcie_pl64g_control_off(dev->pl64g_cap_off);
    uint32_t ctrl = dwc_pcie_cfg_read32(dev, ctrl_off);

    /* Assume bit 0 controls Flit Mode request (platform must confirm). */
    if (enable) { ctrl |=  0x1u; } else { ctrl &= ~0x1u; }
    dwc_pcie_cfg_write32(dev, ctrl_off, ctrl);

    /* Optional: poll status bit 0 to reflect entry/exit */
    uint32_t stat_off = dwc_pcie_pl64g_status_off(dev->pl64g_cap_off);
    const uint32_t poll_us = 1000u;
    uint32_t waited = 0u;
    for (;;) {
        uint32_t sts = dwc_pcie_cfg_read32(dev, stat_off);
        bool in_flit = ((sts & 0x1u) != 0u);
        if ((enable && in_flit) || (!enable && !in_flit)) { break; }
        if (waited >= (dev->timeout_ms * 1000u)) { return DWC_PCIE_E_TIMEOUT; }
        dwc_udelay(poll_us);
        waited += poll_us;
    }
    return DWC_PCIE_OK;
}
