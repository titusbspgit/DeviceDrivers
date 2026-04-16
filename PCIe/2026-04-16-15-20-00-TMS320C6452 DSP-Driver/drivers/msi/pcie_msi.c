#include "pcie_msi.h"
#include "../../hal/hal_mmio.h"
#include "../../include/pcie_regs.h"

static inline uintptr_t reg(pcie_dev_t *d, uint32_t off)
{
    return (d->base + (uintptr_t)off);
}

pcie_status_t pcie_msi_enable(pcie_dev_t *dev, bool enable)
{
    if (dev == NULL) {
        return PCIE_EINVAL;
    }
    if (enable) {
        hal_mmio_rmw32(reg(dev, PCIE_REG_MSI_CTRL), PCIE_MSI_CTRL_EN, 0u);
    } else {
        hal_mmio_rmw32(reg(dev, PCIE_REG_MSI_CTRL), 0u, PCIE_MSI_CTRL_EN);
    }
    return PCIE_OK;
}

pcie_status_t pcie_msi_program(pcie_dev_t *dev, uint32_t addr, uint16_t data)
{
    if (dev == NULL) {
        return PCIE_EINVAL;
    }
    hal_mmio_write32(reg(dev, PCIE_REG_MSI_ADDR), addr);
    hal_mmio_write32(reg(dev, PCIE_REG_MSI_DATA), (uint32_t)data);
    return PCIE_OK;
}
