#include "dw_pcie_link.h"

static void dbi_ro_write_enable(dw_pcie_dev_t *dev, bool en)
{
    uint32_t v = dw_reg_read32(dev->dbi_base, DW_MISC_CONTROL_1_OFF);
    if (en)
    {
        v |= DW_MISC_CTRL_DBI_RO_WR_EN;
    }
    else
    {
        v &= ~DW_MISC_CTRL_DBI_RO_WR_EN;
    }
    dw_reg_write32(dev->dbi_base, DW_MISC_CONTROL_1_OFF, v);
}

int dw_pcie_link_init(dw_pcie_dev_t *dev)
{
    if ((dev == NULL) || (dev->dbi_base == (uintptr_t)0))
    {
        return -1;
    }
    if (dev->pcie_cap == 0u)
    {
        /* attempt to discover */
        (void)dw_pcie_init(dev, dev->dbi_base);
        if (dev->pcie_cap == 0u)
        {
            return -2; /* PCIe capability not found */
        }
    }
    return 0;
}

int dw_pcie_link_get_status(dw_pcie_dev_t *dev, uint8_t *speed, uint8_t *width)
{
    if ((dev == NULL) || (speed == NULL) || (width == NULL))
    {
        return -1;
    }
    if (dev->pcie_cap == 0u)
    {
        return -2;
    }
    uint16_t lnksta = dw_reg_read16(dev->dbi_base, (uint32_t)dev->pcie_cap + PCIE_CAP_REG_LNKSTA);
    *speed = (uint8_t)(lnksta & PCIE_LNKSTA_NEG_LINK_SPEED_MASK);
    *width = (uint8_t)((lnksta >> PCIE_LNKSTA_NEG_LINK_WIDTH_SHIFT) & PCIE_LNKSTA_NEG_LINK_WIDTH_MASK);
    return 0;
}

int dw_pcie_link_set_target_speed(dw_pcie_dev_t *dev, uint8_t target_speed)
{
    if ((dev == NULL) || (dev->pcie_cap == 0u))
    {
        return -1;
    }
    /* Program Target Link Speed in LNKCTL2 (DBI R/O write enable required) */
    dbi_ro_write_enable(dev, true);
    uint16_t lnkctl2 = dw_reg_read16(dev->dbi_base, (uint32_t)dev->pcie_cap + PCIE_CAP_REG_LNKCTL2);
    lnkctl2 = (uint16_t)((lnkctl2 & (uint16_t)(~PCIE_LNKCTL2_TARGET_LINK_SPEED_MASK)) | (uint16_t)(target_speed & 0xFu));
    /* Optionally request Directed Speed Change in LNKCTL2 if bit is defined */
    lnkctl2 = (uint16_t)(lnkctl2 | (uint16_t)PCIE_LNKCTL2_DIRECTED_SPEED_CHANGE);
    dw_reg_write16(dev->dbi_base, (uint32_t)dev->pcie_cap + PCIE_CAP_REG_LNKCTL2, lnkctl2);
    dbi_ro_write_enable(dev, false);

    /* Trigger Directed Speed Change in Port Logic GEN2_CTRL */
    uint32_t gen2 = dw_reg_read32(dev->dbi_base, DW_GEN2_CTRL_OFF);
    gen2 |= DW_GEN2_CTRL_DIRECTED_SPEED_CHANGE;
    dw_reg_write32(dev->dbi_base, DW_GEN2_CTRL_OFF, gen2);

    return 0;
}

int dw_pcie_link_retrain_wait(dw_pcie_dev_t *dev, uint32_t timeout_iters)
{
    if ((dev == NULL) || (dev->pcie_cap == 0u))
    {
        return -1;
    }
    /* Poll until LINK_TRAINING clears and DLL_ACTIVE sets, or timeout */
    uint32_t i;
    for (i = 0u; i < timeout_iters; ++i)
    {
        uint16_t lnksta = dw_reg_read16(dev->dbi_base, (uint32_t)dev->pcie_cap + PCIE_CAP_REG_LNKSTA);
        bool training = ((lnksta & (uint16_t)PCIE_LNKSTA_LINK_TRAINING) != 0u);
        bool dll_active = ((lnksta & (uint16_t)PCIE_LNKSTA_DLL_ACTIVE) != 0u);
        if ((!training) && (dll_active))
        {
            return 0; /* ready */
        }
    }
    return -2; /* timeout */
}
