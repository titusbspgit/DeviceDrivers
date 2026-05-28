#include "keystone_pcie_module_dual_mode_operation_root_complex_and_end_point.h"

static inline int kpm_check_offsets_mode(void)
{
    if (KPM_MODE_REG_OFFSET == UNKNOWN) return KPM_ENOTSUP;
    return KPM_OK;
}

static inline int kpm_check_offsets_ltssm(void)
{
    if (KPM_LTSSM_CTRL_OFFSET == UNKNOWN) return KPM_ENOTSUP;
    return KPM_OK;
}

static inline int kpm_check_offsets_debug(void)
{
    if (KPM_DEBUG_STATE_OFFSET == UNKNOWN) return KPM_ENOTSUP;
    return KPM_OK;
}

int kpm_dual_mode_init(void)
{
    /* No persistent state; validate that required registers are known if needed */
    (void)kpm_check_offsets_mode();
    (void)kpm_check_offsets_ltssm();
    (void)kpm_check_offsets_debug();
    return KPM_OK;
}

int kpm_dual_mode_configure(kpm_pcie_mode_t mode)
{
    if (kpm_check_offsets_mode() != KPM_OK) return KPM_ENOTSUP;
    if (KPM_MODE_MASK == UNKNOWN || KPM_MODE_RC_VAL == UNKNOWN || KPM_MODE_EP_VAL == UNKNOWN)
        return KPM_ENOTSUP;

    uint32_t v = kpm_mmio_read32_be(KPM_BASE_ADDR + (uintptr_t)KPM_MODE_REG_OFFSET);

    /* Clear mode bits */
    if (KPM_MODE_MASK != UNKNOWN)
        v &= ~KPM_MODE_MASK;

    /* Set desired mode */
    if (mode == KPM_PCIE_MODE_RC) {
        v |= KPM_MODE_RC_VAL;
    } else if (mode == KPM_PCIE_MODE_EP) {
        v |= KPM_MODE_EP_VAL;
    } else {
        return KPM_EINVAL;
    }

    kpm_mmio_write32_be(KPM_BASE_ADDR + (uintptr_t)KPM_MODE_REG_OFFSET, v);

    return KPM_OK;
}

int kpm_dual_mode_get_mode(kpm_pcie_mode_t *mode)
{
    if (!mode) return KPM_EINVAL;
    if (kpm_check_offsets_mode() != KPM_OK) return KPM_ENOTSUP;
    if (KPM_MODE_MASK == UNKNOWN || KPM_MODE_RC_VAL == UNKNOWN || KPM_MODE_EP_VAL == UNKNOWN)
        return KPM_ENOTSUP;

    uint32_t v = kpm_mmio_read32_be(KPM_BASE_ADDR + (uintptr_t)KPM_MODE_REG_OFFSET);
    uint32_t m = v & KPM_MODE_MASK;

    if (m == KPM_MODE_RC_VAL) {
        *mode = KPM_PCIE_MODE_RC;
        return KPM_OK;
    } else if (m == KPM_MODE_EP_VAL) {
        *mode = KPM_PCIE_MODE_EP;
        return KPM_OK;
    }

    return KPM_EIO; /* Unknown field value */
}

int kpm_dual_mode_enable_ltssm(void)
{
    if (kpm_check_offsets_ltssm() != KPM_OK) return KPM_ENOTSUP;
    if (KPM_LTSSM_EN_BIT == UNKNOWN) return KPM_ENOTSUP;

    uint32_t v = kpm_mmio_read32_be(KPM_BASE_ADDR + (uintptr_t)KPM_LTSSM_CTRL_OFFSET);
    v |= KPM_LTSSM_EN_BIT;
    kpm_mmio_write32_be(KPM_BASE_ADDR + (uintptr_t)KPM_LTSSM_CTRL_OFFSET, v);
    return KPM_OK;
}

int kpm_dual_mode_disable_ltssm(void)
{
    if (kpm_check_offsets_ltssm() != KPM_OK) return KPM_ENOTSUP;
    if (KPM_LTSSM_EN_BIT == UNKNOWN) return KPM_ENOTSUP;

    uint32_t v = kpm_mmio_read32_be(KPM_BASE_ADDR + (uintptr_t)KPM_LTSSM_CTRL_OFFSET);
    v &= ~KPM_LTSSM_EN_BIT;
    kpm_mmio_write32_be(KPM_BASE_ADDR + (uintptr_t)KPM_LTSSM_CTRL_OFFSET, v);
    return KPM_OK;
}

int kpm_dual_mode_get_link_state(uint32_t *state_raw, bool *in_l0)
{
    if (state_raw) *state_raw = 0u;
    if (in_l0) *in_l0 = false;

    if (kpm_check_offsets_debug() != KPM_OK) return KPM_ENOTSUP;
    if (KPM_DEBUG_LTSSM_STATE_MASK == UNKNOWN || KPM_DEBUG_STATE_L0_VAL == UNKNOWN)
        return KPM_ENOTSUP;

    uint32_t v = kpm_mmio_read32_be(KPM_BASE_ADDR + (uintptr_t)KPM_DEBUG_STATE_OFFSET);
    if (state_raw) *state_raw = v;

    bool l0 = ((v & KPM_DEBUG_LTSSM_STATE_MASK) == KPM_DEBUG_STATE_L0_VAL);
    if (in_l0) *in_l0 = l0;

    return KPM_OK;
}
