#include "test_common.h"
#include "dw_pcie_iatu.h"

int main(void)
{
    TEST_INFO("DW PCIe iATU Outbound Test (big-endian HAL)");

    for (size_t i = 0; i < (sizeof(g_fake_dbi)/sizeof(g_fake_dbi[0])); ++i) { g_fake_dbi[i] = 0u; }

    dw_pcie_dev_t dev;
    (void)dw_pcie_init(&dev, fake_dbi_base());

    uint64_t cpu_addr = 0x80000000ull;
    uint64_t pci_addr = 0x00000010ull;
    uint64_t size     = 0x00100000ull; /* 1MB */

    int rc = dw_pcie_iatu_program_ob(&dev, 0u, DW_IATU_TLP_MEM, cpu_addr, pci_addr, size);
    TEST_ASSERT(rc == 0);

    /* Validate registers */
    TEST_ASSERT(dw_reg_read32(dev.dbi_base, DW_IATU_VIEWPORT_OFF) == 0u);
    TEST_ASSERT(dw_reg_read32(dev.dbi_base, DW_IATU_REGION_CTRL1_OFF) == DW_IATU_TYPE_MEM);
    TEST_ASSERT(dw_reg_read32(dev.dbi_base, DW_IATU_LWR_BASE_ADDR_OFF) == (uint32_t)cpu_addr);
    TEST_ASSERT(dw_reg_read32(dev.dbi_base, DW_IATU_UPPER_BASE_ADDR_OFF) == (uint32_t)(cpu_addr >> 32));
    TEST_ASSERT(dw_reg_read32(dev.dbi_base, DW_IATU_LIMIT_ADDR_OFF) == (uint32_t)((cpu_addr + size - 1ull) & 0xFFFFFFFFull));
    TEST_ASSERT(dw_reg_read32(dev.dbi_base, DW_IATU_LWR_TARGET_ADDR_OFF) == (uint32_t)pci_addr);
    TEST_ASSERT(dw_reg_read32(dev.dbi_base, DW_IATU_UPPER_TARGET_ADDR_OFF) == (uint32_t)(pci_addr >> 32));
    TEST_ASSERT((dw_reg_read32(dev.dbi_base, DW_IATU_REGION_CTRL2_OFF) & DW_IATU_CTRL2_REGION_EN) != 0u);

    TEST_INFO("iATU test PASS");
    return 0;
}
