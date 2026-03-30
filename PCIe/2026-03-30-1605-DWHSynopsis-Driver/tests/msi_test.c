#include "test_common.h"
#include "dw_pcie_msi.h"

int main(void)
{
    TEST_INFO("DW PCIe MSI Test (big-endian HAL)");

    for (size_t i = 0; i < (sizeof(g_fake_dbi)/sizeof(g_fake_dbi[0])); ++i) { g_fake_dbi[i] = 0u; }

    dw_pcie_dev_t dev;
    (void)dw_pcie_init(&dev, fake_dbi_base());

    /* Configure a dummy MSI doorbell address */
    uint64_t doorbell = 0xFEE00000ull; /* sample address (platform-specific) */
    dw_pcie_msi_set_address(&dev, doorbell);
    TEST_ASSERT(dw_reg_read32(dev.dbi_base, DW_MSI_ADDR_LO_OFF) == (uint32_t)(doorbell & 0xFFFFFFFFu));
    TEST_ASSERT(dw_reg_read32(dev.dbi_base, DW_MSI_ADDR_HI_OFF) == (uint32_t)(doorbell >> 32));

    /* Enable vector 3 */
    TEST_ASSERT(dw_pcie_msi_enable_vector(&dev, 3u) == 0);
    TEST_ASSERT((dw_reg_read32(dev.dbi_base, DW_MSI_INTR0_ENABLE_OFF) & (1u << 3)) != 0u);
    TEST_ASSERT((dw_reg_read32(dev.dbi_base, DW_MSI_INTR0_MASK_OFF) & (1u << 3)) == 0u);

    /* Simulate interrupt status and clear it */
    dw_reg_write32(dev.dbi_base, DW_MSI_INTR0_STATUS_OFF, 0u); /* clear any */
    /* Set bit 3 by writing device-side value directly through HAL conversion */
    dw_reg_write32(dev.dbi_base, DW_MSI_INTR0_STATUS_OFF, (1u << 3)); /* W1C will clear only on next write */
    uint32_t st = dw_pcie_msi_read_status(&dev, 0u);
    TEST_ASSERT((st & (1u << 3)) != 0u);
    dw_pcie_msi_clear_status(&dev, 0u, (1u << 3));
    st = dw_pcie_msi_read_status(&dev, 0u);
    TEST_ASSERT((st & (1u << 3)) == 0u);

    /* Disable vector 3 */
    TEST_ASSERT(dw_pcie_msi_disable_vector(&dev, 3u) == 0);
    TEST_ASSERT((dw_reg_read32(dev.dbi_base, DW_MSI_INTR0_ENABLE_OFF) & (1u << 3)) == 0u);

    TEST_INFO("MSI test PASS");
    return 0;
}
