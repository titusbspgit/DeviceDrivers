/* Driver: Initialization sequence (RC mode) */
#include "keystone_pcie_module_initialization_sequence_rc_mode.h"

int kpm_init_rc_sequence(const struct pcie_core_api *api, const struct kpm_init_rc_params *p)
{
    /* 1) Enable SerDes PLL and wait for lock */
    /* Note: these are chip-level addresses */
    volatile uint32_t *pll = (volatile uint32_t *)(REG_SERDES_CFGPLL_ADDR);
    volatile uint32_t *sts = (volatile uint32_t *)(REG_SERDES_STS_ADDR);
    *pll |= SERDES_CFGPLL_ENPLL;
    api->delay(1000);
    uint32_t loops = 1000000u;
    while (loops--) {
        if ((*sts & SERDES_STS_LOCK) != 0u) break;
    }

    /* 2) Program common clock if requested */
    if (p && p->common_clock) {
        uint32_t v = api->read32(REG_LINK_STAT_CTRL_OFF);
        v |= LINK_STAT_COMMON_CLK_CFG;
        api->write32(REG_LINK_STAT_CTRL_OFF, v);
    }

    /* 3) Request target speed */
    if (p && p->request_gen2) {
        uint32_t v = api->read32(REG_LINK_CTRL2_OFF);
        v &= ~LINK_CTRL2_TGT_SPEED_MASK;
        v |= LINK_CTRL2_TGT_SPEED_GEN2;
        api->write32(REG_LINK_CTRL2_OFF, v);
    } else {
        uint32_t v = api->read32(REG_LINK_CTRL2_OFF);
        v &= ~LINK_CTRL2_TGT_SPEED_MASK;
        v |= LINK_CTRL2_TGT_SPEED_GEN1;
        api->write32(REG_LINK_CTRL2_OFF, v);
    }

    /* 4) Enable LTSSM */
    uint32_t v = api->read32(REG_CMD_STATUS_OFF);
    v |= CMD_STATUS_LTSSM_EN;
    api->write32(REG_CMD_STATUS_OFF, v);

    /* 5) Wait for link up */
    if (api->poll_bits_all_clear(REG_LINK_STAT_CTRL_OFF, LINK_STAT_TRAINING, 1000000))
        return -1;
    if (api->poll_bits_all_set(REG_LINK_STAT_CTRL_OFF, LINK_STAT_DLL_ACTIVE, 1000000))
        return -1;

    return 0;
}
