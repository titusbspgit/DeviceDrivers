#include "KeyStone_PCIe_ModuleLink_width_and_speed_x1_x2_Gen1_Gen2.h"

static inline int check_regs(void)
{
    if (KPM_LINK_CTRL_OFFSET  == KPM_REG_UNKNOWN) return KPM_ENOTSUP;
    if (KPM_LINK_CTRL2_OFFSET == KPM_REG_UNKNOWN) return KPM_ENOTSUP;
    if (KPM_LINK_STAT_OFFSET  == KPM_REG_UNKNOWN) return KPM_ENOTSUP;
    if (KPM_LINK_CTRL_WIDTH_MASK     == KPM_REG_UNKNOWN) return KPM_ENOTSUP;
    if (KPM_LINK_CTRL2_SPEED_MASK    == KPM_REG_UNKNOWN) return KPM_ENOTSUP;
    if (KPM_LINK_STAT_NEG_WIDTH_MASK == KPM_REG_UNKNOWN) return KPM_ENOTSUP;
    if (KPM_LINK_STAT_SPEED_MASK     == KPM_REG_UNKNOWN) return KPM_ENOTSUP;
    return KPM_OK;
}

int kpm_linkws_init(const kpm_core_api_t *core)
{
    if (!core || !core->read32 || !core->write32) return KPM_EINVAL;
    (void)check_regs();
    return KPM_OK;
}

int kpm_linkws_set_advertised(const kpm_core_api_t *core, kpm_link_width_t width, kpm_link_speed_t speed)
{
    if (!core || !core->read32 || !core->write32) return KPM_EINVAL;
    int st = check_regs();
    if (st != KPM_OK) return st;

    uint32_t lc = core->read32(KPM_LINK_CTRL_OFFSET);
    uint32_t lc2 = core->read32(KPM_LINK_CTRL2_OFFSET);

    lc  &= ~KPM_LINK_CTRL_WIDTH_MASK;
    lc2 &= ~KPM_LINK_CTRL2_SPEED_MASK;

    switch (width) {
    case KPM_LINK_WIDTH_X1: lc |= KPM_LINK_CTRL_WIDTH_X1; break;
    case KPM_LINK_WIDTH_X2: lc |= KPM_LINK_CTRL_WIDTH_X2; break;
    default: return KPM_EINVAL;
    }

    switch (speed) {
    case KPM_LINK_SPEED_GEN1: lc2 |= KPM_LINK_CTRL2_SPEED_GEN1; break;
    case KPM_LINK_SPEED_GEN2: lc2 |= KPM_LINK_CTRL2_SPEED_GEN2; break;
    default: return KPM_EINVAL;
    }

    core->write32(KPM_LINK_CTRL_OFFSET, lc);
    core->write32(KPM_LINK_CTRL2_OFFSET, lc2);

    return KPM_OK;
}

int kpm_linkws_get_negotiated(const kpm_core_api_t *core, kpm_link_width_t *width, kpm_link_speed_t *speed)
{
    if (!core) return KPM_EINVAL;
    if (width) *width = (kpm_link_width_t)0;
    if (speed) *speed = (kpm_link_speed_t)0;

    int st = check_regs();
    if (st != KPM_OK) return st;

    uint32_t ls = core->read32(KPM_LINK_STAT_OFFSET);

    if (width) {
        uint32_t w = ls & KPM_LINK_STAT_NEG_WIDTH_MASK;
        if (w == KPM_LINK_STAT_NEG_WIDTH_X1) *width = KPM_LINK_WIDTH_X1;
        else if (w == KPM_LINK_STAT_NEG_WIDTH_X2) *width = KPM_LINK_WIDTH_X2;
        else return KPM_EIO;
    }

    if (speed) {
        uint32_t s = ls & KPM_LINK_STAT_SPEED_MASK;
        if (s == KPM_LINK_STAT_SPEED_GEN1) *speed = KPM_LINK_SPEED_GEN1;
        else if (s == KPM_LINK_STAT_SPEED_GEN2) *speed = KPM_LINK_SPEED_GEN2;
        else return KPM_EIO;
    }

    return KPM_OK;
}
