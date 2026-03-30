/* SPDX-License-Identifier: BSD-3-Clause */
/* RCI-ag-DrvGen Agent: Synopsys DWC PCIe DM v6.00a — ASPM L0s/L1 Control */
#ifndef DW_PCIE_ASPM_H
#define DW_PCIE_ASPM_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* PCIe Capability A+0x10: Link Control/Status */
#define DW_PCIE_CAP_LINK_CTRLSTS_OFF   (0x10u)

/* Link Control bits */
#define DW_PCIE_LC_ASPM_L0S_EN         (1u << 0)
#define DW_PCIE_LC_ASPM_L1_EN          (1u << 1)
#define DW_PCIE_LC_CLKPM_EN            (1u << 8)

/* Link Capabilities A+0x0C: bits 11:10 indicate ASPM support */
#define DW_PCIE_CAP_LINK_CAP_OFF       (0x0Cu)
#define DW_PCIE_LCAP_ASPM_MASK         (0x3u << 10)
#define DW_PCIE_LCAP_ASPM_L0S          (0x1u << 10)
#define DW_PCIE_LCAP_ASPM_L1           (0x2u << 10)
#define DW_PCIE_LCAP_ASPM_L0S_L1       (0x3u << 10)

/* DWC Port Logic tuning */
#define DW_PCIE_ACK_F_ASPM_CTRL_OFF    (0x70Cu)
#define DW_PCIE_PORT_LINK_CTRL_OFF     (0x710u)

#define DW_PCIE_PORT_EXTENDED_SYNCH    (1u << 26)
#define DW_PCIE_ASPM_L1_TIMER_ENABLE   (1u << 31)

/* Context */
typedef struct dw_pcie_aspm_ctx
{
    volatile uint8_t * ecam_base;
    volatile uint8_t * dbi_base;
    uint32_t            pcie_cap_off;
} dw_pcie_aspm_ctx_t;

static inline uint32_t rd32_aspm(const volatile uint8_t *b, uint32_t o)
{ const volatile uint32_t *p=(const volatile uint32_t*)(b+o); return *p; }
static inline void wr32_aspm(volatile uint8_t *b, uint32_t o, uint32_t v)
{ volatile uint32_t *p=(volatile uint32_t*)(b+o); *p=v; }

int32_t dw_pcie_aspm_init(dw_pcie_aspm_ctx_t * ctx, volatile void * ecam_base, volatile void * dbi_base, uint32_t pcie_cap_off);

uint32_t dw_pcie_aspm_link_cap(const dw_pcie_aspm_ctx_t * ctx);
int32_t  dw_pcie_aspm_set(const dw_pcie_aspm_ctx_t * ctx, bool l0s, bool l1);
void     dw_pcie_aspm_tune(dw_pcie_aspm_ctx_t * ctx, bool extended_synch, uint8_t l1_timer_code);

#ifdef __cplusplus
}
#endif

#endif /* DW_PCIE_ASPM_H */
