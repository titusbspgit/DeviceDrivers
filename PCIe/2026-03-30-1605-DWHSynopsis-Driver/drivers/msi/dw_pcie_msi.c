#include "dw_pcie_msi.h"

static inline uint32_t msi_group_off(uint32_t base_off, uint32_t group)
{
    return base_off + (group * DW_MSI_GROUP_STRIDE);
}

int dw_pcie_msi_enable_vector(dw_pcie_dev_t *dev, uint32_t vector)
{
    if (dev == NULL)
    {
        return -1;
    }
    uint32_t group = vector / 32u;
    uint32_t bit   = vector % 32u;
    uint32_t off   = msi_group_off(DW_MSI_INTR0_ENABLE_OFF, group);
    uint32_t cur   = dw_reg_read32(dev->dbi_base, off);
    cur |= (1u << bit);
    dw_reg_write32(dev->dbi_base, off, cur);

    /* Unmask if masks are present */
    uint32_t mask_off = msi_group_off(DW_MSI_INTR0_MASK_OFF, group);
    uint32_t m       = dw_reg_read32(dev->dbi_base, mask_off);
    m &= ~(1u << bit);
    dw_reg_write32(dev->dbi_base, mask_off, m);

    return 0;
}

int dw_pcie_msi_disable_vector(dw_pcie_dev_t *dev, uint32_t vector)
{
    if (dev == NULL)
    {
        return -1;
    }
    uint32_t group = vector / 32u;
    uint32_t bit   = vector % 32u;
    uint32_t off   = msi_group_off(DW_MSI_INTR0_ENABLE_OFF, group);
    uint32_t cur   = dw_reg_read32(dev->dbi_base, off);
    cur &= ~(1u << bit);
    dw_reg_write32(dev->dbi_base, off, cur);

    /* Mask it as well */
    uint32_t mask_off = msi_group_off(DW_MSI_INTR0_MASK_OFF, group);
    uint32_t m       = dw_reg_read32(dev->dbi_base, mask_off);
    m |= (1u << bit);
    dw_reg_write32(dev->dbi_base, mask_off, m);

    return 0;
}

uint32_t dw_pcie_msi_read_status(dw_pcie_dev_t *dev, uint32_t group)
{
    uint32_t off = msi_group_off(DW_MSI_INTR0_STATUS_OFF, group);
    return dw_reg_read32(dev->dbi_base, off);
}

void dw_pcie_msi_clear_status(dw_pcie_dev_t *dev, uint32_t group, uint32_t mask)
{
    uint32_t off = msi_group_off(DW_MSI_INTR0_STATUS_OFF, group);
    dw_reg_write1c(dev->dbi_base, off, mask);
}
