/*
 * Synopsys DesignWare PCIe Embedded Host Controller (DM) v6.00a
 * Link Training and Equalization Controls - HAL/Driver Public API
 * RCI-ag-DrvGen Agent
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef DW_PCIE_EQ_DBG_H
#define DW_PCIE_EQ_DBG_H

#include <stdint.h>
#include <stdbool.h>

#define DWC_PCIE_DSP_GEN3_RELATED_OFF                 (0x0890u)
#define DWC_PCIE_DSP_GEN3_EQ_LOCAL_FS_LF_OFF          (0x0894u)
#define DWC_PCIE_DSP_GEN3_EQ_PSET_COEFF_MAP_OFF       (0x0898u)
#define DWC_PCIE_DSP_GEN3_EQ_PSET_INDEX_OFF           (0x089Cu)
#define DWC_PCIE_DSP_GEN3_EQ_COEFF_LEGAL_STATUS_OFF   (0x08A4u)
#define DWC_PCIE_DSP_GEN3_EQ_CONTROL_OFF              (0x08A8u)
#define DWC_PCIE_DSP_VENDOR_SPEC_DLLP_OFF             (0x0704u)
#define DWC_PCIE_DSP_PORT_FORCE_OFF                   (0x0708u)
#define DWC_PCIE_MISC_CONTROL_1_OFF                   (0x08BCu)
#define DWC_PCIE_MISC_CONTROL_1_DBI_RO_WR_EN          (1u << 0)

#define GEN3_RELATED_EQ_PHASE23_DISABLE               (1u << 9)
#define GEN3_RELATED_EQ_REDO_DISABLE                  (1u << 11)
#define GEN3_RELATED_RXEQ_EVAL_REGARDLESS_RXTS        (1u << 13)
#define GEN3_RELATED_RE_EQ_REQUEST_ENABLE             (1u << 19)
#define GEN3_RELATED_AUTO_EQ_DISABLE                  (1u << 21)
#define GEN3_RELATED_USP_SEND_8GT_EQ_TS2_DISABLE      (1u << 22)
#define GEN3_RELATED_INVREQ_EVAL_DIFF_DISABLE         (1u << 23)

#define GEN3_EQ_LOCAL_LF_MASK                         (0x3Fu)
#define GEN3_EQ_LOCAL_LF_SHIFT                        (0u)
#define GEN3_EQ_LOCAL_FS_MASK                         (0x3Fu)
#define GEN3_EQ_LOCAL_FS_SHIFT                        (6u)

#define GEN3_EQ_PSET_PRE_MASK                         (0x3Fu)
#define GEN3_EQ_PSET_PRE_SHIFT                        (0u)
#define GEN3_EQ_PSET_CUR_MASK                         (0x3Fu)
#define GEN3_EQ_PSET_CUR_SHIFT                        (6u)
#define GEN3_EQ_PSET_POST_MASK                        (0x3Fu)
#define GEN3_EQ_PSET_POST_SHIFT                       (12u)

#define GEN3_EQ_CTRL_PVEC_MASK                        (0x07FFu)
#define GEN3_EQ_CTRL_PVEC_SHIFT                       (0u)

#ifndef DLLP_TYPE_SHIFT
#define DLLP_TYPE_SHIFT                               (0u)
#endif
#ifndef DLLP_LEN_SHIFT
#define DLLP_LEN_SHIFT                                (8u)
#endif
#ifndef DLLP_GO_BIT
#define DLLP_GO_BIT                                   (1u << 23)
#endif
#ifndef DLLP_DONE_BIT
#define DLLP_DONE_BIT                                 (1u << 28)
#endif
#ifndef DLLP_ERR_BIT
#define DLLP_ERR_BIT                                  (1u << 29)
#endif
#ifndef DLLP_BUSY_BIT
#define DLLP_BUSY_BIT                                 (1u << 30)
#endif

#ifndef PORT_FORCE_LINK_RECOVERY
#define PORT_FORCE_LINK_RECOVERY                      (1u << 0)
#endif
#ifndef PORT_FORCE_LINK_DISABLE
#define PORT_FORCE_LINK_DISABLE                       (1u << 1)
#endif
#ifndef PORT_FORCE_LINK_DETECT
#define PORT_FORCE_LINK_DETECT                        (1u << 2)
#endif

typedef struct dw_pcie_eq_hal_ops_tag {
    uint32_t (*rd32)(uintptr_t base, uint32_t offset);
    void     (*wr32)(uintptr_t base, uint32_t offset, uint32_t value);
    void     (*udelay)(uint32_t usec);
    int      (*get_link_training_dllla)(void *user, bool *training, bool *dllla);
} dw_pcie_eq_hal_ops_t;

typedef struct dw_pcie_eq_ctx_tag {
    uintptr_t                dbi_base;
    void                    *user;
    const dw_pcie_eq_hal_ops_t *ops;
    uint32_t                 retrain_timeout_us;
    uint32_t                 eq_timeout_us;
} dw_pcie_eq_ctx_t;

int dw_pcie_eq_init(dw_pcie_eq_ctx_t *ctx,
                    uintptr_t dbi_base,
                    void *user_cookie,
                    const dw_pcie_eq_hal_ops_t *ops);

int dw_pcie_eq_read_gen3_related(const dw_pcie_eq_ctx_t *ctx, uint32_t *value);
int dw_pcie_eq_write_gen3_related(const dw_pcie_eq_ctx_t *ctx, uint32_t value, bool protected_write);

int dw_pcie_eq_get_local_fs_lf(const dw_pcie_eq_ctx_t *ctx, uint8_t *fs, uint8_t *lf);
int dw_pcie_eq_set_local_fs_lf(const dw_pcie_eq_ctx_t *ctx, uint8_t fs, uint8_t lf, bool protected_write);

int dw_pcie_eq_program_preset(const dw_pcie_eq_ctx_t *ctx, uint8_t preset_index,
                              uint8_t pre, uint8_t cursor, uint8_t post,
                              bool protected_write);
int dw_pcie_eq_read_preset(const dw_pcie_eq_ctx_t *ctx, uint8_t preset_index,
                           uint8_t *pre, uint8_t *cursor, uint8_t *post);

int dw_pcie_eq_set_preset_vector(const dw_pcie_eq_ctx_t *ctx, uint16_t preset_vec_mask, bool protected_write);

int dw_pcie_eq_read_coeff_legal_status(const dw_pcie_eq_ctx_t *ctx, uint32_t *status_dw);

int dw_pcie_eq_force_recovery(const dw_pcie_eq_ctx_t *ctx);
int dw_pcie_eq_force_detect(const dw_pcie_eq_ctx_t *ctx);
int dw_pcie_eq_force_disable(const dw_pcie_eq_ctx_t *ctx);

int dw_pcie_eq_enter_recovery_and_wait(const dw_pcie_eq_ctx_t *ctx);

int dw_pcie_eq_send_vendor_dllp(const dw_pcie_eq_ctx_t *ctx,
                                uint8_t type, uint8_t length,
                                uint32_t payload_dw0, uint32_t payload_dw1);

int dw_pcie_eq_protected_write32(const dw_pcie_eq_ctx_t *ctx, uint32_t offset, uint32_t value);

static inline uint32_t dw_pcie_pack_fs_lf(uint8_t fs, uint8_t lf)
{
    const uint32_t fsf = ((uint32_t)(fs & GEN3_EQ_LOCAL_FS_MASK)) << GEN3_EQ_LOCAL_FS_SHIFT;
    const uint32_t lff = ((uint32_t)(lf & GEN3_EQ_LOCAL_LF_MASK)) << GEN3_EQ_LOCAL_LF_SHIFT;
    return (fsf | lff);
}

static inline uint32_t dw_pcie_pack_pset(uint8_t pre, uint8_t cursor, uint8_t post)
{
    const uint32_t pref = ((uint32_t)(pre & GEN3_EQ_PSET_PRE_MASK)) << GEN3_EQ_PSET_PRE_SHIFT;
    const uint32_t curf = ((uint32_t)(cursor & GEN3_EQ_PSET_CUR_MASK)) << GEN3_EQ_PSET_CUR_SHIFT;
    const uint32_t postf= ((uint32_t)(post & GEN3_EQ_PSET_POST_MASK)) << GEN3_EQ_PSET_POST_SHIFT;
    return (pref | curf | postf);
}

#endif /* DW_PCIE_EQ_DBG_H */
