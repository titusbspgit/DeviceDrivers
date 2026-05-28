#include "keystone_pcie_module_dual_mode_operation_rc_ep.h"

static inline bool regs_known(void)
{
    return (KPM_REG_DEVSTAT_OFFSET != 0xFFFFFFFFu);
}

int kpm_dual_mode_init(const KPM_CoreApi *api)
{
    if (!api || !api->read32 || !api->write32) return KPM_ERR_INVAL;
    if (!regs_known()) return KPM_ERR_UNSUPPORTED;
    /* No specific initialization sequence defined without known registers */
    return KPM_OK;
}

int kpm_dual_mode_set_role(const KPM_CoreApi *api, KPM_DualModeRole role)
{
    if (!api || !api->read32 || !api->write32) return KPM_ERR_INVAL;
    if (!regs_known()) return KPM_ERR_UNSUPPORTED;

    uint32_t v = api->read32(api->base, KPM_REG_DEVSTAT_OFFSET);
    v &= ~KPM_DEVSTAT_PCIESSMODE_MASK;
    v |= ((uint32_t)role << KPM_DEVSTAT_PCIESSMODE_SHIFT) & KPM_DEVSTAT_PCIESSMODE_MASK;
    api->write32(api->base, KPM_REG_DEVSTAT_OFFSET, v);
    (void)api->read32(api->base, KPM_REG_DEVSTAT_OFFSET); /* post-write read */
    return KPM_OK;
}

int kpm_dual_mode_get_role(const KPM_CoreApi *api, KPM_DualModeRole *role)
{
    if (!api || !api->read32 || !role) return KPM_ERR_INVAL;
    if (!regs_known()) return KPM_ERR_UNSUPPORTED;

    uint32_t v = api->read32(api->base, KPM_REG_DEVSTAT_OFFSET);
    v = (v & KPM_DEVSTAT_PCIESSMODE_MASK) >> KPM_DEVSTAT_PCIESSMODE_SHIFT;
    *role = (v ? KPM_ROLE_EP : KPM_ROLE_RC); /* UNKNOWN mapping; best-effort */
    return KPM_OK;
}
