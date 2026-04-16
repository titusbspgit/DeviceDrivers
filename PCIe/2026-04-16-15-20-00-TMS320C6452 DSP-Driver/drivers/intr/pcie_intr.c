#include "pcie_intr.h"
#include "../../hal/hal_mmio.h"
#include "../../include/pcie_regs.h"

static inline uintptr_t reg(pcie_dev_t *d, uint32_t off)
{
    return (d->base + (uintptr_t)off);
}

pcie_status_t pcie_intr_enable_mask(pcie_dev_t *dev, uint32_t mask)
{
    if (dev == NULL) {
        return PCIE_EINVAL;
    }
    uint32_t v = hal_mmio_read32(reg(dev, PCIE_REG_INT_MASK));
    v |= mask;
    hal_mmio_write32(reg(dev, PCIE_REG_INT_MASK), v);
    return PCIE_OK;
}

pcie_status_t pcie_intr_disable_mask(pcie_dev_t *dev, uint32_t mask)
{
    if (dev == NULL) {
        return PCIE_EINVAL;
    }
    uint32_t v = hal_mmio_read32(reg(dev, PCIE_REG_INT_MASK));
    v &= ~mask;
    hal_mmio_write32(reg(dev, PCIE_REG_INT_MASK), v);
    return PCIE_OK;
}

uint32_t pcie_intr_status(pcie_dev_t *dev)
{
    if (dev == NULL) {
        return 0u;
    }
    return hal_mmio_read32(reg(dev, PCIE_REG_INT_STATUS));
}

pcie_status_t pcie_intr_clear(pcie_dev_t *dev, uint32_t mask)
{
    if (dev == NULL) {
        return PCIE_EINVAL;
    }
    /* W1C */
    hal_mmio_write32(reg(dev, PCIE_REG_INT_STATUS), mask);
    return PCIE_OK;
}
