#include "KeyStone_PCIe_ModuleDual_mode_operation_RC_EP.h"

static inline int check_mode_regs(void)
{
    if (KPM_MODE_REG_OFFSET == KPM_REG_UNKNOWN) return KPM_ENOTSUP;
    if (KPM_MODE_MASK == KPM_REG_UNKNOWN) return KPM_ENOTSUP;
    if (KPM_MODE_RC_VAL == KPM_REG_UNKNOWN) return KPM_ENOTSUP;
    if (KPM_MODE_EP_VAL == KPM_REG_UNKNOWN) return KPM_ENOTSUP;
    return KPM_OK;
}

static inline int check_ltssm_regs(void)
{
    if (KPM_LTSSM_CTRL_OFFSET == KPM_REG_UNKNOWN) return KPM_ENOTSUP;
    if (KPM_LTSSM_EN_BIT == KPM_REG_UNKNOWN) return KPM_ENOTSUP;
    return KPM_OK;
}

static inline int check_debug_regs(void)
{
    if (KPM_DEBUG_STATE_OFFSET == KPM_REG_UNKNOWN) return KPM_ENOTSUP;
    if (KPM_DEBUG_LTSSM_STATE_MASK == KPM_REG_UNKNOWN) return KPM_ENOTSUP;
    if (KPM_DEBUG_STATE_L0_VAL == KPM_REG_UNKNOWN) return KPM_ENOTSUP;
    return KPM_OK;
}

int kpm_dualmode_init(const kpm_core_api_t *core)
{
    if (!core || !core->read32 || !core->write32) return KPM_EINVAL;
    /* No hardware side effects here; validation only */
    (void)check_mode_regs();
    (void)check_ltssm_regs();
    (void)check_debug_regs();
    return KPM_OK;
}

int kpm_dualmode_set_mode(const kpm_core_api_t *core, kpm_pcie_mode_t mode)
{
    if (!core || !core->read32 || !core->write32) return KPM_EINVAL;
    int st = check_mode_regs();
    if (st != KPM_OK) return st;

    uint32_t v = core->read32(KPM_MODE_REG_OFFSET);
    v &= ~KPM_MODE_MASK;
    if (mode == KPM_PCIE_MODE_RC)      v |= KPM_MODE_RC_VAL;
    else if (mode == KPM_PCIE_MODE_EP) v |= KPM_MODE_EP_VAL;
    else return KPM_EINVAL;

    core->write32(KPM_MODE_REG_OFFSET, v);
    return KPM_OK;
}

int kpm_dualmode_get_mode(const kpm_core_api_t *core, kpm_pcie_mode_t *mode)
{
    if (!core || !mode) return KPM_EINVAL;
    int st = check_mode_regs();
    if (st != KPM_OK) return st;

    uint32_t v = core->read32(KPM_MODE_REG_OFFSET) & KPM_MODE_MASK;
    if (v == KPM_MODE_RC_VAL)      { *mode = KPM_PCIE_MODE_RC; return KPM_OK; }
    if (v == KPM_MODE_EP_VAL)      { *mode = KPM_PCIE_MODE_EP; return KPM_OK; }
    return KPM_EIO;
}

int kpm_dualmode_enable_ltssm(const kpm_core_api_t *core)
{
    if (!core) return KPM_EINVAL;
    int st = check_ltssm_regs();
    if (st != KPM_OK) return st;

    uint32_t v = core->read32(KPM_LTSSM_CTRL_OFFSET);
    v |= KPM_LTSSM_EN_BIT;
    core->write32(KPM_LTSSM_CTRL_OFFSET, v);
    return KPM_OK;
}

int kpm_dualmode_disable_ltssm(const kpm_core_api_t *core)
{
    if (!core) return KPM_EINVAL;
    int st = check_ltssm_regs();
    if (st != KPM_OK) return st;

    uint32_t v = core->read32(KPM_LTSSM_CTRL_OFFSET);
    v &= ~KPM_LTSSM_EN_BIT;
    core->write32(KPM_LTSSM_CTRL_OFFSET, v);
    return KPM_OK;
}

int kpm_dualmode_get_link_state(const kpm_core_api_t *core, uint32_t *state_raw, bool *in_l0)
{
    if (!core) return KPM_EINVAL;
    if (state_raw) *state_raw = 0u;
    if (in_l0) *in_l0 = false;

    int st = check_debug_regs();
    if (st != KPM_OK) return st;

    uint32_t v = core->read32(KPM_DEBUG_STATE_OFFSET);
    if (state_raw) *state_raw = v;
    if (in_l0) *in_l0 = ((v & KPM_DEBUG_LTSSM_STATE_MASK) == KPM_DEBUG_STATE_L0_VAL);
    return KPM_OK;
}

int kpm_dualmode_wait_l0(const kpm_core_api_t *core, uint32_t max_iters)
{
    if (!core) return KPM_EINVAL;
    int st = check_debug_regs();
    if (st != KPM_OK) return st;

    for (uint32_t i = 0; i < max_iters; ++i) {
        uint32_t v = core->read32(KPM_DEBUG_STATE_OFFSET);
        if ((v & KPM_DEBUG_LTSSM_STATE_MASK) == KPM_DEBUG_STATE_L0_VAL)
            return KPM_OK;
    }
    return KPM_ETIME;
}
