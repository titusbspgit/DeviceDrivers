#include "keystone_pcie_module_link_width_and_speed_x1_x2_gen1_gen2.h"

static inline bool regs_known(void)
{
    return (KPM_REG_LINK_CTRL_OFFSET != 0xFFFFFFFFu) &&
           (KPM_REG_LINK_STATUS_OFFSET != 0xFFFFFFFFu);
}

int kpm_link_init(const KPM_CoreApi *api)
{
    if (!api || !api->read32 || !api->write32) return KPM_ERR_INVAL;
    if (!regs_known()) return KPM_ERR_UNSUPPORTED;
    /* No further initialization without known register map */
    return KPM_OK;
}

int kpm_link_request(const KPM_CoreApi *api, KPM_LinkWidth width, KPM_LinkSpeed speed)
{
    if (!api || !api->read32 || !api->write32) return KPM_ERR_INVAL;
    if (!regs_known()) return KPM_ERR_UNSUPPORTED;

    uint32_t v = api->read32(api->base, KPM_REG_LINK_CTRL_OFFSET);
    v &= ~(KPM_LINK_CTRL_TARGET_SPEED_MASK | KPM_LINK_CTRL_WIDTH_MASK);
    v |= (((uint32_t)speed << KPM_LINK_CTRL_TARGET_SPEED_SHIFT) & KPM_LINK_CTRL_TARGET_SPEED_MASK);
    v |= (((uint32_t)width << KPM_LINK_CTRL_WIDTH_SHIFT) & KPM_LINK_CTRL_WIDTH_MASK);
    api->write32(api->base, KPM_REG_LINK_CTRL_OFFSET, v);
    (void)api->read32(api->base, KPM_REG_LINK_CTRL_OFFSET);
    return KPM_OK;
}

int kpm_link_query(const KPM_CoreApi *api, KPM_LinkWidth *width, KPM_LinkSpeed *speed)
{
    if (!api || !api->read32 || !width || !speed) return KPM_ERR_INVAL;
    if (!regs_known()) return KPM_ERR_UNSUPPORTED;

    uint32_t s = api->read32(api->base, KPM_REG_LINK_STATUS_OFFSET);
    *speed = (KPM_LinkSpeed)((s & KPM_LINK_STATUS_SPEED_MASK) >> KPM_LINK_STATUS_SPEED_SHIFT);
    *width = (KPM_LinkWidth)((s & KPM_LINK_STATUS_WIDTH_MASK) >> KPM_LINK_STATUS_WIDTH_SHIFT);
    return KPM_OK;
}
