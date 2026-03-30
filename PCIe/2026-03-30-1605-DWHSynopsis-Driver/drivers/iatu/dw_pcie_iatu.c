#include "dw_pcie_iatu.h"

static void iatu_select_viewport(dw_pcie_dev_t *dev, uint32_t index, dw_iatu_dir_t dir)
{
    uint32_t v = (index & DW_IATU_VIEWPORT_REGION_IDX_MASK);
    if (dir == DW_IATU_DIR_INBOUND)
    {
        v |= DW_IATU_VIEWPORT_DIR_INBOUND;
    }
    dw_reg_write32(dev->dbi_base, DW_IATU_VIEWPORT_OFF, v);
}

int dw_pcie_iatu_program_ob(dw_pcie_dev_t *dev,
                            uint32_t region_index,
                            dw_iatu_tlp_t tlp_type,
                            uint64_t cpu_addr,
                            uint64_t pci_addr,
                            uint64_t size)
{
    if ((dev == NULL) || (tlp_type > DW_IATU_TLP_CFG1) || (size == 0u))
    {
        return -1;
    }
    if (region_index >= DW_PCIE_IATU_MAX_OB_REGIONS)
    {
        return -2;
    }

    /* Select outbound region */
    iatu_select_viewport(dev, region_index, DW_IATU_DIR_OUTBOUND);

    /* Program CTRL1: TLP type */
    dw_reg_write32(dev->dbi_base, DW_IATU_REGION_CTRL1_OFF, (uint32_t)tlp_type);

    /* Program base/limit/target */
    uint32_t cpu_lo = (uint32_t)(cpu_addr & 0xFFFFFFFFu);
    uint32_t cpu_hi = (uint32_t)((cpu_addr >> 32) & 0xFFFFFFFFu);
    uint32_t tgt_lo = (uint32_t)(pci_addr & 0xFFFFFFFFu);
    uint32_t tgt_hi = (uint32_t)((pci_addr >> 32) & 0xFFFFFFFFu);
    uint64_t limit  = cpu_addr + (size - 1u);
    uint32_t limit_lo = (uint32_t)(limit & 0xFFFFFFFFu);

    dw_reg_write32(dev->dbi_base, DW_IATU_LWR_BASE_ADDR_OFF, cpu_lo);
    dw_reg_write32(dev->dbi_base, DW_IATU_UPPER_BASE_ADDR_OFF, cpu_hi);
    dw_reg_write32(dev->dbi_base, DW_IATU_LIMIT_ADDR_OFF, limit_lo);
    dw_reg_write32(dev->dbi_base, DW_IATU_LWR_TARGET_ADDR_OFF, tgt_lo);
    dw_reg_write32(dev->dbi_base, DW_IATU_UPPER_TARGET_ADDR_OFF, tgt_hi);

    /* Enable region */
    dw_reg_write32(dev->dbi_base, DW_IATU_REGION_CTRL2_OFF, DW_IATU_CTRL2_REGION_EN);

    /* Confirm region enable (poll a few times) */
    for (uint32_t i = 0u; i < 1000u; ++i)
    {
        uint32_t st = dw_reg_read32(dev->dbi_base, DW_IATU_REGION_CTRL2_OFF);
        if ((st & DW_IATU_CTRL2_REGION_EN) != 0u)
        {
            break;
        }
    }

    DW_DSB();
    return 0;
}
