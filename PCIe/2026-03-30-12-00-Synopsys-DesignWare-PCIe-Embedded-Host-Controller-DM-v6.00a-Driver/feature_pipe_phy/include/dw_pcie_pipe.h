/* SPDX-License-Identifier: BSD-3-Clause */
/* RCI-ag-DrvGen Agent: Synopsys DWC PCIe DM v6.00a — PIPE PHY Interface */
#ifndef DW_PCIE_PIPE_H
#define DW_PCIE_PIPE_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Offsets (Port Logic / DSP base) */
#define DW_PCIE_PHY_VIEWPORT_CTLSTS_OFF   (0xB70u)
#define DW_PCIE_PHY_VIEWPORT_DATA_OFF     (0xB74u)
#define DW_PCIE_PIPE_RELATED_OFF          (0xB90u)
#define DW_PCIE_PHY_STATUS_OFF            (0x810u)
#define DW_PCIE_PHY_CONTROL_OFF           (0x814u)
#define DW_PCIE_GEN2_CTRL_OFF             (0x80Cu)

/* Viewport CTLSTS bits (assumed; verify on your IP): */
#define DW_PHY_VP_BUSY                    (1u << 31)
#define DW_PHY_VP_START                   (1u << 0)
#define DW_PHY_VP_RW                      (1u << 1) /* 1: write, 0: read */
#define DW_PHY_VP_ADDR_SHIFT              (2u)
#define DW_PHY_VP_ADDR_MASK               (0x3FFu << DW_PHY_VP_ADDR_SHIFT)

/* PHY_STATUS bits (assumed): */
#define DW_PHY_STATUS_READY               (1u << 0)

/* Basic context */
typedef struct dw_pcie_pipe_ctx
{
    volatile uint8_t * dbi_base; /* Port Logic base */
} dw_pcie_pipe_ctx_t;

static inline uint32_t dw_mmio_read32_pl(volatile const uint8_t * base, uint32_t off)
{ const volatile uint32_t *p=(const volatile uint32_t*)(base+off); return *p; }
static inline void     dw_mmio_write32_pl(volatile uint8_t * base, uint32_t off, uint32_t v)
{ volatile uint32_t *p=(volatile uint32_t*)(base+off); *p = v; }

int32_t dw_pcie_pipe_init(dw_pcie_pipe_ctx_t * ctx, volatile void * dbi_base);

/* PHY viewport accessors (blocking, bounded polling) */
int32_t dw_pcie_phy_write(dw_pcie_pipe_ctx_t * ctx, uint16_t addr, uint32_t data, uint32_t timeout_loops);
int32_t dw_pcie_phy_read (dw_pcie_pipe_ctx_t * ctx, uint16_t addr, uint32_t * data_out, uint32_t timeout_loops);

/* Simple bring-up helpers */
bool    dw_pcie_phy_ready(const dw_pcie_pipe_ctx_t * ctx);
void    dw_pcie_pipe_config_msg_bus(dw_pcie_pipe_ctx_t * ctx, uint8_t rx_wb_depth, uint8_t tx_min_wb_depth);

#ifdef __cplusplus
}
#endif

#endif /* DW_PCIE_PIPE_H */
