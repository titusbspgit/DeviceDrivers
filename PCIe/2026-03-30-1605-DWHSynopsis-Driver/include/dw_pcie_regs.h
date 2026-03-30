#ifndef DW_PCIE_REGS_H
#define DW_PCIE_REGS_H

/* Port Logic / DBI Offsets (commonly documented) */
#define DW_VEND_SPEC_DLLP_OFF          (0x704u)
#define DW_PORT_FORCE_OFF              (0x708u)
#define DW_ACK_F_ASPM_CTRL_OFF         (0x70Cu)
#define DW_PORT_LINK_CTRL_OFF          (0x710u)
#define DW_GEN2_CTRL_OFF               (0x80Cu)
#define DW_PHY_STATUS_OFF              (0x810u)
#define DW_PHY_CONTROL_OFF             (0x814u)
#define DW_MISC_CONTROL_1_OFF          (0x8BCu) /* DBI RO write enable */
#define DW_PCIE_VERSION_NUMBER_OFF     (0x8F8u)
#define DW_PCIE_VERSION_TYPE_OFF       (0x8FCu)

/* MISC_CONTROL_1 fields */
#define DW_MISC_CTRL_DBI_RO_WR_EN      (0x1u)

/* GEN2_CTRL fields (subset) */
#define DW_GEN2_CTRL_DIRECTED_SPEED_CHANGE   (1u << 17)

/* iATU Viewport model */
#define DW_IATU_VIEWPORT_OFF           (0x900u)
#define DW_IATU_REGION_CTRL1_OFF       (0x904u)
#define DW_IATU_REGION_CTRL2_OFF       (0x908u)
#define DW_IATU_LWR_BASE_ADDR_OFF      (0x90Cu)
#define DW_IATU_UPPER_BASE_ADDR_OFF    (0x910u)
#define DW_IATU_LIMIT_ADDR_OFF         (0x914u)
#define DW_IATU_LWR_TARGET_ADDR_OFF    (0x918u)
#define DW_IATU_UPPER_TARGET_ADDR_OFF  (0x91Cu)

/* iATU Viewport fields */
#define DW_IATU_VIEWPORT_REGION_IDX_MASK   (0xFFu)
#define DW_IATU_VIEWPORT_DIR_INBOUND       (1u << 31)

/* iATU CTRL1 TYPE encodings (common) */
#define DW_IATU_TYPE_MEM               (0x0u)
#define DW_IATU_TYPE_IO                (0x2u)
#define DW_IATU_TYPE_CFG0              (0x4u)
#define DW_IATU_TYPE_CFG1              (0x5u)

/* iATU CTRL2 fields */
#define DW_IATU_CTRL2_REGION_EN        (1u << 31)
#define DW_IATU_CTRL2_BAR_MATCH_EN     (1u << 30)

/* PCI Express Capability IDs and offsets within the capability */
#define PCI_CAP_ID_EXP                 (0x10u)

#define PCIE_CAP_REG_LNKCAP            (0x0Cu)
#define PCIE_CAP_REG_LNKCTL            (0x10u)
#define PCIE_CAP_REG_LNKSTA            (0x12u)
#define PCIE_CAP_REG_LNKCAP2           (0x2Cu)
#define PCIE_CAP_REG_LNKCTL2           (0x30u)
#define PCIE_CAP_REG_LNKSTA2           (0x32u)

/* LNKCTL2 fields */
#define PCIE_LNKCTL2_TARGET_LINK_SPEED_MASK   (0xFu)

/* LNKSTA fields */
#define PCIE_LNKSTA_NEG_LINK_SPEED_MASK       (0xFu)
#define PCIE_LNKSTA_NEG_LINK_WIDTH_SHIFT      (4u)
#define PCIE_LNKSTA_NEG_LINK_WIDTH_MASK       (0x3Fu)
#define PCIE_LNKSTA_LINK_TRAINING             (1u << 11)
#define PCIE_LNKSTA_DLL_ACTIVE                (1u << 13)

/* MSI host block */
#define DW_MSI_ADDR_LO_OFF              (0x820u)
#define DW_MSI_ADDR_HI_OFF              (0x824u)
#define DW_MSI_INTR0_ENABLE_OFF         (0x828u)
#define DW_MSI_INTR0_MASK_OFF           (0x82Cu)
#define DW_MSI_INTR0_STATUS_OFF         (0x830u)

/* Per-group stride for MSI registers */
#define DW_MSI_GROUP_STRIDE             (0x0Cu)

#endif /* DW_PCIE_REGS_H */
