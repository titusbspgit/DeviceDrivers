/*
 * Core driver for KeyStone PCIe Module
 * arch=arm env=Baremetal compiler=gcc endianness=bigendian
 */
#ifndef KEYSTONE_PCIE_MODULE_CORE_DRIVER_H
#define KEYSTONE_PCIE_MODULE_CORE_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

/* Base address */
#define PCIE_BASE_ADDR        (0xA1008000u)

/* MMIO helpers */
static inline volatile uint32_t *pcie_reg32(uint32_t off)
{ return (volatile uint32_t *)(PCIE_BASE_ADDR + off); }

static inline uint32_t pcie_read32(uint32_t off)
{ return *pcie_reg32(off); }

static inline void pcie_write32(uint32_t off, uint32_t val)
{ *pcie_reg32(off) = val; }

static inline void pcie_set_bits(uint32_t off, uint32_t mask)
{ *pcie_reg32(off) |= mask; }

static inline void pcie_clr_bits(uint32_t off, uint32_t mask)
{ *pcie_reg32(off) &= ~mask; }

/* Basic delay (busy-wait) - cycles not calibrated */
static inline void pcie_delay(volatile uint32_t cycles)
{ while (cycles--) { __asm__ __volatile__("" ::: "memory"); } }

/* Key registers and fields (from TI KeyStone PCIe module cheat sheet) */
#define REG_CMD_STATUS_OFF            0x0004u
#define  CMD_STATUS_LTSSM_EN          (1u << 0)
#define  CMD_STATUS_OB_XLT_EN         (1u << 1)
#define  CMD_STATUS_IB_XLT_EN         (1u << 2)
#define  CMD_STATUS_APP_RETRY_EN      (1u << 4)

#define REG_LINK_STAT_CTRL_OFF        0x1080u
#define  LINK_STAT_TRAINING           (1u << 26)       /* R */
#define  LINK_STAT_DLL_ACTIVE         (1u << 27)       /* R */
#define  LINK_STAT_NEG_WIDTH_SHIFT    20               /* R [25:20] */
#define  LINK_STAT_NEG_WIDTH_MASK     (0x3Fu << LINK_STAT_NEG_WIDTH_SHIFT)
#define  LINK_STAT_SPEED_SHIFT        16               /* R [19:16] */
#define  LINK_STAT_SPEED_MASK         (0xFu << LINK_STAT_SPEED_SHIFT)
#define  LINK_STAT_COMMON_CLK_CFG     (1u << 6)        /* R/W */
#define  LINK_STAT_RETRAIN_LINK       (1u << 5)        /* W1S */
#define  LINK_STAT_HW_AUTO_WD_DIS     (1u << 9)        /* R/W */
#define  LINK_STAT_CLK_PWR_MGMT_EN    (1u << 8)        /* R/W */

#define REG_LINK_CTRL2_OFF            0x10A0u
#define  LINK_CTRL2_TGT_SPEED_MASK    (0xFu)
#define  LINK_CTRL2_TGT_SPEED_GEN1    (0x1u)
#define  LINK_CTRL2_TGT_SPEED_GEN2    (0x2u)
#define  LINK_CTRL2_SEL_DEEMPH        (1u << 6)

/* MSI capability / local application block */
#define REG_MSI_CAP_OFF               0x1050u
#define REG_MSI_ADDR_LOW_OFF          0x1054u
#define REG_MSI_ADDR_UP_OFF           0x1058u
#define REG_MSI_DATA_OFF              0x105Cu

#define REG_MSI_DOORBELL_OFF          0x0054u /* Write vector (0..31) to inject */

/* MSI per-vector groups: base = 0xA1008100 + n*0x20 (n=0..7) */
#define MSI_GROUP_STRIDE              0x20u
#define MSI_GROUP_BASE0_OFF           0x0100u
#define MSI_RAW_STATUS_OFF            0x0u
#define MSI_STATUS_OFF                0x4u
#define MSI_ENABLE_SET_OFF            0x8u
#define MSI_ENABLE_CLR_OFF            0xCu

/* Legacy INTx (example: INTA) */
#define REG_INTX_RAW_STATUS_OFF       0x0180u
#define REG_INTX_STATUS_OFF           0x0184u
#define REG_INTX_ENABLE_SET_OFF       0x0188u
#define REG_INTX_ENABLE_CLR_OFF       0x018Cu
#define REG_INTX_EOI_OFF              0x0050u

/* SerDes PLL (chip-level) */
#define REG_SERDES_CFGPLL_ADDR        0x02620358u
#define  SERDES_CFGPLL_ENPLL          (1u << 0)
#define REG_SERDES_STS_ADDR           0x0262015Cu
#define  SERDES_STS_LOCK              (1u << 0)

/* Core API exposed to feature drivers */
struct pcie_core_api {
    uint32_t (*read32)(uint32_t off);
    void     (*write32)(uint32_t off, uint32_t val);
    void     (*set_bits)(uint32_t off, uint32_t mask);
    void     (*clr_bits)(uint32_t off, uint32_t mask);
    void     (*delay)(uint32_t cycles);
    /* Common helpers */
    int      (*poll_bits_all_set)(uint32_t off, uint32_t mask, uint32_t timeout_loops);
    int      (*poll_bits_all_clear)(uint32_t off, uint32_t mask, uint32_t timeout_loops);
};

/* Get initialized core API */
const struct pcie_core_api *pcie_core_get_api(void);

/* Convenience inline wrappers */
static inline uint32_t pcie_link_speed(void)
{
    return (pcie_read32(REG_LINK_STAT_CTRL_OFF) & LINK_STAT_SPEED_MASK) >> LINK_STAT_SPEED_SHIFT;
}

static inline uint32_t pcie_link_width(void)
{
    return (pcie_read32(REG_LINK_STAT_CTRL_OFF) & LINK_STAT_NEG_WIDTH_MASK) >> LINK_STAT_NEG_WIDTH_SHIFT;
}

#endif /* KEYSTONE_PCIE_MODULE_CORE_DRIVER_H */
