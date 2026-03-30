/* SPDX-License-Identifier: BSD-3-Clause */
/* RCI-ag-DrvGen Agent: Synopsys DWC PCIe DM v6.00a — Link Training & EQ Controls */
#ifndef DW_PCIE_EQ_H
#define DW_PCIE_EQ_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* DWC_PCIE_DSP offsets */
#define DW_PCIE_GEN3_RELATED_OFF          (0x890u)
#define DW_PCIE_GEN3_EQ_LOCAL_FS_LF_OFF   (0x894u)
#define DW_PCIE_GEN3_EQ_PSET_COEF_MAP_OFF (0x898u)
#define DW_PCIE_GEN3_EQ_PSET_INDEX_OFF    (0x89Cu)
#define DW_PCIE_VENDOR_SPEC_DLLP_OFF      (0x704u)
#define DW_PCIE_PORT_FORCE_OFF            (0x708u)

/* Fields (assumed; verify on your IP): */
#define DW_PCIE_GEN3_EQ_LOCAL_LF_MASK     (0x3Fu)
#define DW_PCIE_GEN3_EQ_LOCAL_FS_MASK     (0x3Fu)
#define DW_PCIE_GEN3_EQ_LOCAL_FS_SHIFT    (6u)

#define DW_PCIE_GEN3_EQ_PSET_PRE_MASK     (0x3Fu)
#define DW_PCIE_GEN3_EQ_PSET_CUR_MASK     (0x3Fu)
#define DW_PCIE_GEN3_EQ_PSET_CUR_SHIFT    (6u)
#define DW_PCIE_GEN3_EQ_PSET_POST_MASK    (0x3Fu)
#define DW_PCIE_GEN3_EQ_PSET_POST_SHIFT   (12u)

#define DW_PCIE_DLLP_START                (1u << 0)
#define DW_PCIE_DLLP_BUSY                 (1u << 31)

#define DW_PCIE_PORT_FORCE_RETRAIN        (1u << 5)  /* Example bit for retrain */

/* Context */
typedef struct dw_pcie_eq_ctx
{
    volatile uint8_t * dbi_base; /* DWC_PCIE_DSP base */
} dw_pcie_eq_ctx_t;

static inline uint32_t rd32_eq(const volatile uint8_t *b, uint32_t o)
{ const volatile uint32_t *p=(const volatile uint32_t*)(b+o); return *p; }
static inline void wr32_eq(volatile uint8_t *b, uint32_t o, uint32_t v)
{ volatile uint32_t *p=(volatile uint32_t*)(b+o); *p=v; }

int32_t dw_pcie_eq_init(dw_pcie_eq_ctx_t * ctx, volatile void * dbi_base);

/* Local FS/LF (receiver hints, Gen3) */
void    dw_pcie_eq_set_local_fs_lf(dw_pcie_eq_ctx_t * ctx, uint8_t fs, uint8_t lf);
void    dw_pcie_eq_get_local_fs_lf(const dw_pcie_eq_ctx_t * ctx, uint8_t * fs, uint8_t * lf);

/* Preset coefficient map (index-based) */
int32_t dw_pcie_eq_set_preset(dw_pcie_eq_ctx_t * ctx, uint8_t preset_index, uint8_t pre, uint8_t cur, uint8_t post);
int32_t dw_pcie_eq_get_preset(dw_pcie_eq_ctx_t * ctx, uint8_t preset_index, uint8_t * pre, uint8_t * cur, uint8_t * post);

/* Vendor-specific DLLP (simple trigger) */
int32_t dw_pcie_send_vendor_dllp(dw_pcie_eq_ctx_t * ctx, uint32_t payload, uint32_t timeout_loops);

/* Force retrain (guarded) */
void    dw_pcie_force_retrain(dw_pcie_eq_ctx_t * ctx);

#ifdef __cplusplus
}
#endif

#endif /* DW_PCIE_EQ_H */
