#include "pcie_core.h"
#include "../../hal/hal_mmio.h"
#include "../../include/pcie_regs.h"

static inline uintptr_t reg(pcie_dev_t *d, uint32_t off)
{
    return (d->base + (uintptr_t)off);
}

pcie_status_t pcie_core_init(pcie_dev_t *dev)
{
    if (dev == NULL) {
        return PCIE_EINVAL;
    }

    /* Enable core */
    hal_mmio_rmw32(reg(dev, PCIE_REG_CMD), PCIE_CMD_CORE_EN, 0u);

    /* Wait for core ready bit */
    for (uint32_t i = 0u; i < 1000000u; ++i) {
        uint32_t st = hal_mmio_read32(reg(dev, PCIE_REG_STATUS));
        if ((st & PCIE_STATUS_CORE_RDY) != 0u) {
            return PCIE_OK;
        }
    }
    return PCIE_ETIMEOUT;
}

pcie_status_t pcie_core_soft_reset(pcie_dev_t *dev, uint32_t timeout_cycles)
{
    if ((dev == NULL) || (timeout_cycles == 0u)) {
        return PCIE_EINVAL;
    }

    /* Assert reset */
    hal_mmio_rmw32(reg(dev, PCIE_REG_CMD), PCIE_CMD_CORE_RST, 0u);
    /* Small delay loop */
    for (volatile uint32_t dly = 0u; dly < 1000u; ++dly) {
        __asm__ volatile ("" : : : "memory");
    }
    /* Deassert reset */
    hal_mmio_rmw32(reg(dev, PCIE_REG_CMD), 0u, PCIE_CMD_CORE_RST);

    /* Re-enable core */
    hal_mmio_rmw32(reg(dev, PCIE_REG_CMD), PCIE_CMD_CORE_EN, 0u);

    for (uint32_t i = 0u; i < timeout_cycles; ++i) {
        uint32_t st = hal_mmio_read32(reg(dev, PCIE_REG_STATUS));
        if ((st & PCIE_STATUS_CORE_RDY) != 0u) {
            return PCIE_OK;
        }
    }
    return PCIE_ETIMEOUT;
}

pcie_status_t pcie_core_get_id(pcie_dev_t *dev, uint32_t *out_id)
{
    if ((dev == NULL) || (out_id == NULL)) {
        return PCIE_EINVAL;
    }
    *out_id = hal_mmio_read32(reg(dev, PCIE_REG_ID));
    return PCIE_OK;
}

bool pcie_core_is_ready(pcie_dev_t *dev)
{
    if (dev == NULL) {
        return false;
    }
    return (hal_mmio_read32(reg(dev, PCIE_REG_STATUS)) & PCIE_STATUS_CORE_RDY) != 0u;
}
