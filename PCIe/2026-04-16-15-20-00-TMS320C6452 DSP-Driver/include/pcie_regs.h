/*
 * pcie_regs.h - Assumed PCIe controller register map (DWC-like) for TMS320C6452 DSP integration
 * NOTE: This is a conservative, minimal map for driver scaffolding. Replace with exact spec values via RAG.
 */
#ifndef PCIE_REGS_H
#define PCIE_REGS_H

#include <stdint.h>
#include <stddef.h>

/* Common register offsets from base (assumptions) */
#define PCIE_REG_ID                 (0x000u)  /* RO: Identification/Version */
#define PCIE_REG_CMD                (0x004u)  /* RW: Command/Control */
#define PCIE_REG_STATUS             (0x008u)  /* RO: Status */

/* Link Training and Status */
#define PCIE_REG_LTSSM_CTRL         (0x100u)  /* RW: LTSSM control */
#define PCIE_REG_LTSSM_STATUS       (0x104u)  /* RO: LTSSM state/Link up */

/* Configuration access (Type 0/1) */
#define PCIE_REG_CFG_ADDR           (0x200u)  /* RW: CFG address composition */
#define PCIE_REG_CFG_DATA           (0x204u)  /* RW: CFG data window */

/* MSI control */
#define PCIE_REG_MSI_CTRL           (0x300u)  /* RW: MSI enable/mask */
#define PCIE_REG_MSI_ADDR           (0x304u)  /* RW: MSI address (lower 32 bits) */
#define PCIE_REG_MSI_DATA           (0x308u)  /* RW: MSI data payload */

/* Interrupts */
#define PCIE_REG_INT_STATUS         (0x400u)  /* RW1C: Interrupt status */
#define PCIE_REG_INT_MASK           (0x404u)  /* RW: Interrupt mask/enable */

/* Simple DMA (if present) */
#define PCIE_REG_DMA_CTRL           (0x500u)  /* RW: DMA control/status */
#define PCIE_REG_DMA_SRC            (0x504u)  /* RW: DMA source address */
#define PCIE_REG_DMA_DST            (0x508u)  /* RW: DMA dest address */
#define PCIE_REG_DMA_LEN            (0x50Cu)  /* RW: DMA length (bytes) */

/* PCIE_REG_CMD bitfields */
#define PCIE_CMD_CORE_EN            (1u << 0)
#define PCIE_CMD_CORE_RST           (1u << 1)
#define PCIE_CMD_LTSSM_EN           (1u << 2)

/* STATUS bits */
#define PCIE_STATUS_CORE_RDY        (1u << 0)

/* LTSSM */
#define PCIE_LTSSM_CTRL_START       (1u << 0)
#define PCIE_LTSSM_STATUS_LINK_UP   (1u << 0)

/* CFG address composition (assumed):
 * [31]      : Type (0=Type0, 1=Type1)
 * [30:24]   : Bus number
 * [23:19]   : Device number
 * [18:16]   : Function number
 * [15:8]    : Register number (DWORD aligned)
 * [1:0]     : Byte offset within DWORD
 */
#define PCIE_CFG_ADDR_TYPE1         (1u << 31)
#define PCIE_CFG_ADDR_BUS_SHIFT     (24u)
#define PCIE_CFG_ADDR_DEV_SHIFT     (19u)
#define PCIE_CFG_ADDR_FUN_SHIFT     (16u)
#define PCIE_CFG_ADDR_REG_SHIFT     (8u)
#define PCIE_CFG_ADDR_BYTE_SHIFT    (0u)

/* MSI */
#define PCIE_MSI_CTRL_EN            (1u << 0)

/* Interrupts (example bits) */
#define PCIE_INT_LINK               (1u << 0)
#define PCIE_INT_MSI                (1u << 1)
#define PCIE_INT_DMA                (1u << 2)

/* DMA */
#define PCIE_DMA_CTRL_START         (1u << 0)
#define PCIE_DMA_CTRL_BUSY          (1u << 31)

#endif /* PCIE_REGS_H */
