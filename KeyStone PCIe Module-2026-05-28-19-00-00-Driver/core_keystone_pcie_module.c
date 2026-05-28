#include "core_keystone_pcie_module.h"

static int core_init(void)
{
    /* Non-destructive probe */
    (void)pcie_read32(PCIE_CMD_STATUS_OFF);
    return 0;
}

static int core_enable_ltssm(bool en)
{
    uint32_t v = pcie_read32(PCIE_CMD_STATUS_OFF);
    if (en) v |= CMD_STATUS_LTSSM_EN; else v &= ~CMD_STATUS_LTSSM_EN;
    pcie_write32(PCIE_CMD_STATUS_OFF, v);
    return 0;
}

static uint32_t core_get_ltssm_state(void)
{
    /* Placeholder: actual LTSSM state is SoC-specific; return 0 for now */
    return 0u;
}

static int core_set_link_speed_gen2(bool en)
{
    uint32_t v = pcie_read32(PCIE_LINK_CTRL2_OFF);
    if (en) {
        v &= ~0xFu;              /* clear target speed field (placeholder) */
        v |= LINK_CTRL2_TARGET_GEN2;
    } else {
        v &= ~0xFu;              /* set to Gen1 (0x1) if needed */
        v |= 0x1u;
    }
    pcie_write32(PCIE_LINK_CTRL2_OFF, v);
    return 0;
}

static int core_set_common_clock(bool en)
{
    uint32_t v = pcie_read32(PCIE_LINK_STAT_CTRL_OFF);
    if (en) v |= LINK_STAT_CTRL_COMMON_CLK; else v &= ~LINK_STAT_CTRL_COMMON_CLK;
    pcie_write32(PCIE_LINK_STAT_CTRL_OFF, v);
    return 0;
}

static int core_msi_enable_group(unsigned group, bool en)
{
    (void)group; /* single group placeholder */
    uint32_t off = en ? PCIE_MSI_EN_SET_G0_OFF : PCIE_MSI_EN_CLR_G0_OFF;
    pcie_write32(off, 0xFFFFFFFFu);
    return 0;
}

static int core_msi_ack(unsigned vector)
{
    (void)vector; /* acknowledge via EOI: vector semantics platform-specific */
    pcie_write32(PCIE_IRQ_EOI_OFF, 0x1u);
    return 0;
}

static int core_endianness_set(bool big_endian)
{
    (void)big_endian; /* mapping assumed big-endian; no-op */
    return 0;
}

static uint32_t core_reg_read(uint32_t off)
{
    return pcie_read32(off);
}

static void core_reg_write(uint32_t off, uint32_t val)
{
    pcie_write32(off, val);
}

static const pcie_core_ops_t g_ops = {
    .init = core_init,
    .enable_ltssm = core_enable_ltssm,
    .get_ltssm_state = core_get_ltssm_state,
    .set_link_speed_gen2 = core_set_link_speed_gen2,
    .set_common_clock = core_set_common_clock,
    .msi_enable_group = core_msi_enable_group,
    .msi_ack = core_msi_ack,
    .endianness_set = core_endianness_set,
    .reg_read = core_reg_read,
    .reg_write = core_reg_write,
};

const pcie_core_ops_t* pcie_core_get_ops(void)
{
    return &g_ops;
}
