#include "test_common.h"
#include "dw_pcie_msi.h"
#include "dw_pcie_regs.h"

/* Simulate device-side set of W1C status: write device-endian value directly */
static void fake_device_set_status(uintptr_t dbi, uint32_t status_off, uint32_t mask)
{
    volatile uint32_t *addr = (volatile uint32_t *)(dbi + (uintptr_t)status_off);
#if (DW_PCIE_CPU_BIG_ENDIAN == 1u) && (DW_PCIE_DEVICE_LITTLE_ENDIAN == 1u)
    uint32_t cur = __builtin_bswap32(*addr);
    cur |= mask;
    *addr = __builtin_bswap32(cur);
#else
    uint32_t cur = *addr;
    cur |= mask;
    *addr = cur;
#endif
}

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

    /* Simulate interrupt status set by device and clear it */
    fake_device_set_status(dev.dbi_base, DW_MSI_INTR0_STATUS_OFF, (1u << 3));
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
