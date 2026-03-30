/*
 * Synopsys DesignWare PCIe (DWC) – Bare-metal driver header
 * Controller: DWHSynopsis (DesignWare PCIe Embedded Host Controller)
 * Target: ARM (big-endian CPU), Bare metal, GCC
 * Author: RCI-ag-DrvGen Agent
 */
#ifndef DWC_PCIE_DWHSYNOPSIS_H
#define DWC_PCIE_DWHSYNOPSIS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* =========================
 * DBI/Port Logic offsets
 * ========================= */
#define DWC_PCIE_PL_VENDOR_SPEC_DLLP_OFF      (0x704u)
#define DWC_PCIE_PL_PORT_FORCE_OFF            (0x708u)
#define DWC_PCIE_PL_ACK_F_ASPM_CTRL_OFF       (0x70Cu)
#define DWC_PCIE_PL_PORT_LINK_CTRL_OFF        (0x710u)
#define DWC_PCIE_PL_PORT_DEBUG0_OFF           (0x728u)
#define DWC_PCIE_PL_GEN2_CTRL_OFF             (0x80Cu)
#define DWC_PCIE_PL_PHY_STATUS_OFF            (0x810u)
#define DWC_PCIE_PL_MISC_CONTROL_1_OFF        (0x8BCu)

/* PHY_STATUS bits (typical) */
#define DWC_PCIE_PL_PHY_STATUS_PHY_RDY_MASK   (0x00000001u)

/* MISC_CONTROL_1 bits */
#define DWC_PCIE_PL_MISC_CONTROL_1_DBI_RO_WR_EN_MASK (0x00000001u)

/* GEN2_CTRL bits */
#define DWC_PCIE_PL_GEN2_CTRL_DIR_SPEED_CHANGE_MASK  (1u << 17)

/* =========================
 * iATU (viewport model) offsets
 * ========================= */
#define DWC_PCIE_IATU_VIEWPORT_OFF            (0x900u)
#define DWC_PCIE_IATU_REGION_CTRL1_OFF        (0x904u)
#define DWC_PCIE_IATU_REGION_CTRL2_OFF        (0x908u)
#define DWC_PCIE_IATU_LWR_BASE_ADDR_OFF       (0x90Cu)
#define DWC_PCIE_IATU_UPPER_BASE_ADDR_OFF     (0x910u)
#define DWC_PCIE_IATU_LIMIT_ADDR_OFF          (0x914u)
#define DWC_PCIE_IATU_LWR_TARGET_ADDR_OFF     (0x918u)
#define DWC_PCIE_IATU_UPPER_TARGET_ADDR_OFF   (0x91Cu)
#define DWC_PCIE_IATU_REGION_CTRL3_OFF        (0x93Cu)

/* iATU viewport fields */
#define DWC_PCIE_IATU_VIEWPORT_DIR_INBOUND    (1u << 31)
#define DWC_PCIE_IATU_VIEWPORT_DIR_OUTBOUND   (0u)
#define DWC_PCIE_IATU_VIEWPORT_REGION_INDEX_MASK (0xFu) /* up to 16 regions typical */

/* iATU CR1 TYPE encodings (common across DWC revs) */
#define DWC_PCIE_IATU_CR1_TYPE_MEM            (0x0u)
#define DWC_PCIE_IATU_CR1_TYPE_IO             (0x2u)
#define DWC_PCIE_IATU_CR1_TYPE_CFG0           (0x4u)
#define DWC_PCIE_IATU_CR1_TYPE_CFG1           (0x5u)

/* iATU CR2 control bits */
#define DWC_PCIE_IATU_CR2_REGION_EN_MASK      (1u << 31)
#define DWC_PCIE_IATU_CR2_REGION_ACTIVE_MASK  (1u << 28) /* RO */

/* =========================
 * PCIe Capability and Extended Capability helpers
 * ========================= */
#define DWC_PCIE_PCI_CAP_PTR_OFF              (0x34u)
#define DWC_PCIE_PCI_STATUS_OFF               (0x06u)
#define DWC_PCIE_PCI_STATUS_CAP_LIST_MASK     (1u << 4)
#define DWC_PCIE_CAP_ID_PCIE                  (0x10u)

/* Offsets within the PCIe Capability structure (relative to its base) */
#define DWC_PCIE_CAP_LNKCTL_LNKSTA_OFF        (0x10u)
#define DWC_PCIE_CAP_LNKCTL2_OFF              (0x30u)
#define DWC_PCIE_CAP_LNKSTA_OFF               (0x12u)

/* Link Control 2 Target Link Speed encodings (per PCIe spec) */
#define DWC_PCIE_LINK_SPEED_GEN1              (0x1u)
#define DWC_PCIE_LINK_SPEED_GEN2              (0x2u)
#define DWC_PCIE_LINK_SPEED_GEN3              (0x3u)
#define DWC_PCIE_LINK_SPEED_GEN4              (0x4u)
#define DWC_PCIE_LINK_SPEED_GEN5              (0x5u)
#define DWC_PCIE_LINK_SPEED_GEN6              (0x6u)

/* Link Status field helpers */
#define DWC_PCIE_LNKSTA_SPEED_MASK            (0x000Fu)
#define DWC_PCIE_LNKSTA_NLW_MASK              (0x03F0u) /* bits [9:4] */

/* SPCIE and PL64G (Flit) extended capabilities are optional and SoC-defined.
 * Expose their base offsets via configuration if present. Registers commonly are:
 *  - SPCIE: base + 0x0 = header, +0x4 = LINK_CONTROL3, +0x8 = LANE_ERR_STATUS
 *  - PL64G: base + 0x4 = CAPABILITY, +0x8 = CONTROL, +0xC = STATUS
 */

/* =========================
 * Endianness and MMIO access
 * ========================= */
/* The DWC DBI/PL window is little-endian. On big-endian CPUs, byte-swap MMIO. */
#ifndef DWC_PCIE_MMIO_IS_LE
#define DWC_PCIE_MMIO_IS_LE  (1u)
#endif

#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#define DWC_PCIE_CPU_BE 1
#else
#define DWC_PCIE_CPU_BE 0
#endif

static inline uint32_t dwc_pcie_bswap32(uint32_t v)
{
    return ((v & 0x000000FFu) << 24)
         | ((v & 0x0000FF00u) << 8)
         | ((v & 0x00FF0000u) >> 8)
         | ((v & 0xFF000000u) >> 24);
}

static inline uint32_t dwc_pcie_le32_to_cpu(uint32_t v)
{
#if DWC_PCIE_MMIO_IS_LE && DWC_PCIE_CPU_BE
    return dwc_pcie_bswap32(v);
#else
    return v;
#endif
}

static inline uint32_t dwc_pcie_cpu_to_le32(uint32_t v)
{
#if DWC_PCIE_MMIO_IS_LE && DWC_PCIE_CPU_BE
    return dwc_pcie_bswap32(v);
#else
    return v;
#endif
}

static inline uint32_t dwc_pcie_mmio_read32(uintptr_t base, uint32_t off)
{
    volatile const uint32_t *addr = (volatile const uint32_t *)(base + (uintptr_t)off);
    uint32_t raw = *addr; /* single deterministic read */
    return dwc_pcie_le32_to_cpu(raw);
}

static inline void dwc_pcie_mmio_write32(uintptr_t base, uint32_t off, uint32_t val)
{
    volatile uint32_t *addr = (volatile uint32_t *)(base + (uintptr_t)off);
    *addr = dwc_pcie_cpu_to_le32(val);
    (void)*addr; /* ensure write posting completes on some interconnects */
}

/* =========================
 * Types and error codes
 * ========================= */

typedef enum {
    DWC_PCIE_OK = 0,
    DWC_PCIE_E_TIMEOUT = -1,
    DWC_PCIE_E_PARAM = -2,
    DWC_PCIE_E_UNSUPPORTED = -3,
    DWC_PCIE_E_HW = -4
} dwc_pcie_status_t;

typedef struct {
    uintptr_t dbi_base;           /* DBI base address */
    uint32_t  timeout_ms;         /* Poll timeout for link/ATU ops */
    uint32_t  pl64g_cap_off;      /* Optional: Physical Layer 64G Ext Cap base (0 if N/A) */
    uint32_t  spcie_cap_off;      /* Optional: Secondary PCIe Ext Cap base (0 if N/A) */
    uint32_t  pcie_cap_off;       /* PCIe Capability base offset (0 if unknown) */
} dwc_pcie_dev_t;

/* =========================
 * Platform hooks (weak)
 * ========================= */
__attribute__((weak)) void dwc_pcie_platform_ltssm_enable(const dwc_pcie_dev_t *dev);
__attribute__((weak)) void dwc_udelay(uint32_t usec);

/* =========================
 * Public API
 * ========================= */

/* Initialization: set base and optional caps offsets. If pcie_cap_off is 0, driver will try to discover it. */
void dwc_pcie_init(dwc_pcie_dev_t *dev, uintptr_t dbi_base, uint32_t timeout_ms,
                   uint32_t pcie_cap_off, uint32_t spcie_cap_off, uint32_t pl64g_cap_off);

/* DBI read-only write enable control (use for early config writes if needed) */
void dwc_pcie_dbi_ro_write_enable(dwc_pcie_dev_t *dev, bool enable);

/* Capability discovery helpers */
dwc_pcie_status_t dwc_pcie_find_pcie_cap(dwc_pcie_dev_t *dev, uint32_t *cap_off);
uint32_t dwc_pcie_find_ext_cap(const dwc_pcie_dev_t *dev, uint16_t cap_id);

/* PHY and Link bring-up */
dwc_pcie_status_t dwc_pcie_wait_phy_ready(const dwc_pcie_dev_t *dev, uint32_t timeout_ms);
dwc_pcie_status_t dwc_pcie_set_target_link_speed(const dwc_pcie_dev_t *dev, uint8_t speed_code);
dwc_pcie_status_t dwc_pcie_start_link_training(const dwc_pcie_dev_t *dev);
dwc_pcie_status_t dwc_pcie_wait_link_up(const dwc_pcie_dev_t *dev, uint32_t timeout_ms);

/* iATU viewport configuration (Outbound regions) */
dwc_pcie_status_t dwc_pcie_iatu_outbound_set(const dwc_pcie_dev_t *dev,
    uint32_t region_index,
    uint32_t tlp_type,
    uint64_t cpu_base,
    uint64_t size,
    uint64_t pci_target,
    uint32_t cr1_attrs,
    uint32_t cr2_attrs);

dwc_pcie_status_t dwc_pcie_iatu_outbound_disable(const dwc_pcie_dev_t *dev, uint32_t region_index);

/* ASPM controls */
dwc_pcie_status_t dwc_pcie_aspm_set(const dwc_pcie_dev_t *dev, bool l0s_en, bool l1_en);

/* Gen3/Gen4 equalization local coefficients (coarse helper; values are SoC-tuned) */
void dwc_pcie_gen3_eq_set_local(const dwc_pcie_dev_t *dev, uint8_t fs, uint8_t lf);

/* Secondary PCIe Extended Capability helpers (if available) */
static inline uint32_t dwc_pcie_spcie_link_control3_off(uint32_t spcie_base) { return spcie_base + 0x4u; }
static inline uint32_t dwc_pcie_spcie_lane_err_status_off(uint32_t spcie_base) { return spcie_base + 0x8u; }

/* Flit Mode (PL64G) helpers (if available) */
static inline uint32_t dwc_pcie_pl64g_capability_off(uint32_t pl64g_base) { return pl64g_base + 0x4u; }
static inline uint32_t dwc_pcie_pl64g_control_off(uint32_t pl64g_base) { return pl64g_base + 0x8u; }
static inline uint32_t dwc_pcie_pl64g_status_off(uint32_t pl64g_base) { return pl64g_base + 0xCu; }

dwc_pcie_status_t dwc_pcie_flit_mode_enable(const dwc_pcie_dev_t *dev, bool enable);

#ifdef __cplusplus
}
#endif

#endif /* DWC_PCIE_DWHSYNOPSIS_H */
