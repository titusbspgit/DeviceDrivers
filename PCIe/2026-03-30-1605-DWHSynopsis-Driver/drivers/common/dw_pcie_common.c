#include "dw_pcie_hal.h"
#include "dw_pcie_regs.h"

/* PCI config header offsets */
#define PCI_CFG_STATUS_OFF   (0x06u)
#define PCI_CFG_CAP_PTR_OFF  (0x34u)

/* Find capability by ID in standard PCI config space exposed through DBI */
static uint16_t dw_pcie_find_capability_scan(uintptr_t dbi_base, uint8_t cap_id)
{
    /* Check Capabilities List bit in Status register (optional) */
    (void)dbi_base;
    uint8_t pos = dw_reg_read8(dbi_base, PCI_CFG_CAP_PTR_OFF);
    uint16_t limit = 0u;

    while ((pos >= 0x40u) && (pos < 0xF0u) && (limit < 64u))
    {
        uint8_t id = dw_reg_read8(dbi_base, (uint32_t)pos);
        uint8_t next = dw_reg_read8(dbi_base, (uint32_t)pos + 1u);
        if (id == cap_id)
        {
            return (uint16_t)pos;
        }
        if ((next == 0u) || (next == pos))
        {
            break;
        }
        pos = next;
        limit++;
    }
    return 0u; /* not found */
}

int dw_pcie_init(dw_pcie_dev_t *dev, uintptr_t dbi_base)
{
    if (dev == NULL)
    {
        return -1;
    }
    dev->dbi_base = dbi_base;
    dev->pcie_cap = dw_pcie_find_capability_scan(dbi_base, (uint8_t)PCI_CAP_ID_EXP);
    return 0;
}
