// SPDX-License-Identifier: BSD-3-Clause
/* Portable HAL implementation for EQ controls (can be reused by kernel wrapper) */
#include <stddef.h>
#include "../include/dw_pcie_eq_dbg.h"

static inline uint32_t _rd32(const dw_pcie_eq_ctx_t *ctx, uint32_t off){ return ctx->ops->rd32(ctx->dbi_base, off);} 
static inline void _wr32(const dw_pcie_eq_ctx_t *ctx, uint32_t off, uint32_t v){ ctx->ops->wr32(ctx->dbi_base, off, v);} 

int dw_pcie_eq_init(dw_pcie_eq_ctx_t *ctx, uintptr_t dbi_base, void *user_cookie, const dw_pcie_eq_hal_ops_t *ops){
    if ((ctx == NULL) || (ops == NULL) || (ops->rd32 == NULL) || (ops->wr32 == NULL) || (ops->udelay == NULL)) { return -1; }
    ctx->dbi_base = dbi_base; ctx->user = user_cookie; ctx->ops = ops; ctx->retrain_timeout_us = 100000u; ctx->eq_timeout_us = 24000u; return 0;
}

int dw_pcie_eq_protected_write32(const dw_pcie_eq_ctx_t *ctx, uint32_t offset, uint32_t value){ if (ctx == NULL) { return -1; } uint32_t misc = _rd32(ctx, DWC_PCIE_MISC_CONTROL_1_OFF); _wr32(ctx, DWC_PCIE_MISC_CONTROL_1_OFF, misc | DWC_PCIE_MISC_CONTROL_1_DBI_RO_WR_EN); _wr32(ctx, offset, value); _wr32(ctx, DWC_PCIE_MISC_CONTROL_1_OFF, misc & ~DWC_PCIE_MISC_CONTROL_1_DBI_RO_WR_EN); return 0; }

int dw_pcie_eq_read_gen3_related(const dw_pcie_eq_ctx_t *ctx, uint32_t *value){ if ((ctx == NULL) || (value == NULL)) { return -1; } *value = _rd32(ctx, DWC_PCIE_DSP_GEN3_RELATED_OFF); return 0; }
int dw_pcie_eq_write_gen3_related(const dw_pcie_eq_ctx_t *ctx, uint32_t value, bool protected_write){ if (ctx == NULL) { return -1; } if (protected_write) { return dw_pcie_eq_protected_write32(ctx, DWC_PCIE_DSP_GEN3_RELATED_OFF, value);} _wr32(ctx, DWC_PCIE_DSP_GEN3_RELATED_OFF, value); return 0; }

int dw_pcie_eq_get_local_fs_lf(const dw_pcie_eq_ctx_t *ctx, uint8_t *fs, uint8_t *lf){ if ((ctx == NULL) || (fs == NULL) || (lf == NULL)) { return -1; } uint32_t v = _rd32(ctx, DWC_PCIE_DSP_GEN3_EQ_LOCAL_FS_LF_OFF); *lf = (uint8_t)((v >> GEN3_EQ_LOCAL_LF_SHIFT) & GEN3_EQ_LOCAL_LF_MASK); *fs = (uint8_t)((v >> GEN3_EQ_LOCAL_FS_SHIFT) & GEN3_EQ_LOCAL_FS_MASK); return 0; }
int dw_pcie_eq_set_local_fs_lf(const dw_pcie_eq_ctx_t *ctx, uint8_t fs, uint8_t lf, bool protected_write){ if (ctx == NULL) { return -1; } uint32_t v = dw_pcie_pack_fs_lf(fs, lf); if (protected_write) { return dw_pcie_eq_protected_write32(ctx, DWC_PCIE_DSP_GEN3_EQ_LOCAL_FS_LF_OFF, v);} _wr32(ctx, DWC_PCIE_DSP_GEN3_EQ_LOCAL_FS_LF_OFF, v); return 0; }

static int _write_preset_index(const dw_pcie_eq_ctx_t *ctx, uint8_t preset_index, bool protected_write){ if (preset_index > 10u) { return -2; } if (protected_write) { return dw_pcie_eq_protected_write32(ctx, DWC_PCIE_DSP_GEN3_EQ_PSET_INDEX_OFF, (uint32_t)preset_index);} _wr32(ctx, DWC_PCIE_DSP_GEN3_EQ_PSET_INDEX_OFF, (uint32_t)preset_index); return 0; }

int dw_pcie_eq_program_preset(const dw_pcie_eq_ctx_t *ctx, uint8_t preset_index, uint8_t pre, uint8_t cursor, uint8_t post, bool protected_write){ if (ctx == NULL) { return -1; } if ((pre > 63u) || (cursor > 63u) || (post > 63u)) { return -2; } int r = _write_preset_index(ctx, preset_index, protected_write); if (r != 0) { return r; } uint32_t tuple = dw_pcie_pack_pset(pre, cursor, post); if (protected_write) { return dw_pcie_eq_protected_write32(ctx, DWC_PCIE_DSP_GEN3_EQ_PSET_COEFF_MAP_OFF, tuple);} _wr32(ctx, DWC_PCIE_DSP_GEN3_EQ_PSET_COEFF_MAP_OFF, tuple); return 0; }

int dw_pcie_eq_read_preset(const dw_pcie_eq_ctx_t *ctx, uint8_t preset_index, uint8_t *pre, uint8_t *cursor, uint8_t *post){ if ((ctx == NULL) || (pre == NULL) || (cursor == NULL) || (post == NULL)) { return -1; } if (preset_index > 10u) { return -2; } int r = _write_preset_index(ctx, preset_index, false); if (r != 0) { return r; } uint32_t tuple = _rd32(ctx, DWC_PCIE_DSP_GEN3_EQ_PSET_COEFF_MAP_OFF); *pre = (uint8_t)((tuple >> GEN3_EQ_PSET_PRE_SHIFT) & GEN3_EQ_PSET_PRE_MASK); *cursor = (uint8_t)((tuple >> GEN3_EQ_PSET_CUR_SHIFT) & GEN3_EQ_PSET_CUR_MASK); *post = (uint8_t)((tuple >> GEN3_EQ_PSET_POST_SHIFT) & GEN3_EQ_PSET_POST_MASK); return 0; }

int dw_pcie_eq_set_preset_vector(const dw_pcie_eq_ctx_t *ctx, uint16_t preset_vec_mask, bool protected_write){ if (ctx == NULL) { return -1; } uint32_t v = _rd32(ctx, DWC_PCIE_DSP_GEN3_EQ_CONTROL_OFF); uint32_t newv = (v & ~(GEN3_EQ_CTRL_PVEC_MASK << GEN3_EQ_CTRL_PVEC_SHIFT)) | (((uint32_t)preset_vec_mask & GEN3_EQ_CTRL_PVEC_MASK) << GEN3_EQ_CTRL_PVEC_SHIFT); if (protected_write) { return dw_pcie_eq_protected_write32(ctx, DWC_PCIE_DSP_GEN3_EQ_CONTROL_OFF, newv);} _wr32(ctx, DWC_PCIE_DSP_GEN3_EQ_CONTROL_OFF, newv); return 0; }

int dw_pcie_eq_read_coeff_legal_status(const dw_pcie_eq_ctx_t *ctx, uint32_t *status_dw){ if ((ctx == NULL) || (status_dw == NULL)) { return -1; } *status_dw = _rd32(ctx, DWC_PCIE_DSP_GEN3_EQ_COEFF_LEGAL_STATUS_OFF); return 0; }

static int _force_and_delay(const dw_pcie_eq_ctx_t *ctx, uint32_t mask, uint32_t settle_us){ uint32_t v = _rd32(ctx, DWC_PCIE_DSP_PORT_FORCE_OFF); _wr32(ctx, DWC_PCIE_DSP_PORT_FORCE_OFF, v | mask); ctx->ops->udelay(settle_us); return 0; }
int dw_pcie_eq_force_recovery(const dw_pcie_eq_ctx_t *ctx){ if (ctx == NULL) { return -1; } return _force_and_delay(ctx, PORT_FORCE_LINK_RECOVERY, 100u); }
int dw_pcie_eq_force_detect(const dw_pcie_eq_ctx_t *ctx){ if (ctx == NULL) { return -1; } return _force_and_delay(ctx, PORT_FORCE_LINK_DETECT, 100u); }
int dw_pcie_eq_force_disable(const dw_pcie_eq_ctx_t *ctx){ if (ctx == NULL) { return -1; } return _force_and_delay(ctx, PORT_FORCE_LINK_DISABLE, 100u); }

int dw_pcie_eq_enter_recovery_and_wait(const dw_pcie_eq_ctx_t *ctx){ if (ctx == NULL) { return -1; } (void)dw_pcie_eq_force_recovery(ctx); if (ctx->ops->get_link_training_dllla == NULL) { ctx->ops->udelay(ctx->retrain_timeout_us); return 0; } uint32_t remaining = ctx->retrain_timeout_us; while (remaining > 0u) { bool training = true; bool dllla = false; if (ctx->ops->get_link_training_dllla(ctx->user, &training, &dllla) == 0) { if ((!training) && dllla) { return 0; } } if (remaining <= 1000u) { remaining = 0u; } else { remaining -= 1000u; } ctx->ops->udelay(1000u); } return -2; }

int dw_pcie_eq_send_vendor_dllp(const dw_pcie_eq_ctx_t *ctx, uint8_t type, uint8_t length, uint32_t payload_dw0, uint32_t payload_dw1){ (void)payload_dw0; (void)payload_dw1; if (ctx == NULL) { return -1; } uint32_t ctrl; uint32_t remaining = 10000u; while (remaining > 0u) { ctrl = _rd32(ctx, DWC_PCIE_DSP_VENDOR_SPEC_DLLP_OFF); if ((ctrl & DLLP_BUSY_BIT) == 0u) { break; } ctx->ops->udelay(10u); remaining -= 10u; } if (remaining == 0u) { return -2; } ctrl = (((uint32_t)type) << DLLP_TYPE_SHIFT) | (((uint32_t)length) << DLLP_LEN_SHIFT) | DLLP_GO_BIT; _wr32(ctx, DWC_PCIE_DSP_VENDOR_SPEC_DLLP_OFF, ctrl); remaining = 10000u; while (remaining > 0u) { ctrl = _rd32(ctx, DWC_PCIE_DSP_VENDOR_SPEC_DLLP_OFF); if ((ctrl & DLLP_ERR_BIT) != 0u) { return -3; } if ((ctrl & DLLP_DONE_BIT) != 0u) { return 0; } ctx->ops->udelay(10u); remaining -= 10u; } return -4; }
