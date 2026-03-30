#include "test_common.h"
#include "dw_pcie_regs.h"
#include "dw_pcie_link.h"

/* Helpers to seed capability list in fake DBI */
static void seed_pcie_cap(uintptr_t dbi, uint16_t cap_off, uint8_t neg_speed, uint8_t neg_width)
{
    /* Set Capabilities Pointer at 0x34 */
    dw_reg_write8(dbi, 0x34u, (uint8_t)cap_off);
    /* Capability header: ID=0x10, NEXT=0 */
    dw_reg_write8(dbi, cap_off + 0u, (uint8_t)PCI_CAP_ID_EXP);
    dw_reg_write8(dbi, cap_off + 1u, 0u);
    /* LNKSTA with negotiated speed/width and DLL active, training cleared */
    uint16_t lnksta = (uint16_t)((neg_speed & 0xFu) | ((uint16_t)(neg_width & 0x3Fu) << PCIE_LNKSTA_NEG_LINK_WIDTH_SHIFT) | PCIE_LNKSTA_DLL_ACTIVE);
    dw_reg_write16(dbi, (uint32_t)cap_off + PCIE_CAP_REG_LNKSTA, lnksta);
    /* LNKCTL2 initial */
    dw_reg_write16(dbi, (uint32_t)cap_off + PCIE_CAP_REG_LNKCTL2, 0u);
}

int main(void)
{
    TEST_INFO("DW PCIe Link Test (big-endian HAL)");

    /* Clear fake DBI */
    for (size_t i = 0; i < (sizeof(g_fake_dbi)/sizeof(g_fake_dbi[0])); ++i) { g_fake_dbi[i] = 0u; }

    uintptr_t dbi = fake_dbi_base();
    seed_pcie_cap(dbi, 0x50u, 2u, 1u);

    /* Ensure DBI RO write enable starts cleared */
    dw_reg_write32(dbi, DW_MISC_CONTROL_1_OFF, 0u);

    dw_pcie_dev_t dev;
    (void)dw_pcie_init(&dev, dbi);
    TEST_ASSERT(dev.pcie_cap == 0x50u);

    TEST_INFO("PCIe cap at 0x%02X", dev.pcie_cap);

    TEST_ASSERT(dw_pcie_link_init(&dev) == 0);

    uint8_t spd = 0u, width = 0u;
    TEST_ASSERT(dw_pcie_link_get_status(&dev, &spd, &width) == 0);
    TEST_INFO("Neg speed %u, width x%u", spd, width);
    TEST_ASSERT(spd == 2u);
    TEST_ASSERT(width == 1u);

    /* Request Gen3 */
    TEST_ASSERT(dw_pcie_link_set_target_speed(&dev, 3u) == 0);

    /* Check that LNKCTL2.TARGET_LINK_SPEED updated */
    uint16_t lnkctl2 = dw_reg_read16(dbi, (uint32_t)dev.pcie_cap + PCIE_CAP_REG_LNKCTL2);
    TEST_ASSERT((lnkctl2 & PCIE_LNKCTL2_TARGET_LINK_SPEED_MASK) == 3u);

    /* Check that DIRECTED_SPEED_CHANGE is set in GEN2_CTRL */
    uint32_t gen2 = dw_reg_read32(dbi, DW_GEN2_CTRL_OFF);
    TEST_ASSERT((gen2 & DW_GEN2_CTRL_DIRECTED_SPEED_CHANGE) != 0u);

    /* Simulate retrain completed (already set in seed) */
    TEST_ASSERT(dw_pcie_link_retrain_wait(&dev, 1000u) == 0);

    TEST_INFO("Link test PASS");
    return 0;
}
