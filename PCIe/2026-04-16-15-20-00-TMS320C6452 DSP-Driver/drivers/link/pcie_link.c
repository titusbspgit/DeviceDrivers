#include "pcie_link.h"
#include "../../hal/hal_mmio.h"
#include "../../include/pcie_regs.h"

static inline uintptr_t reg(pcie_dev_t *d, uint32_t off)
{
    return (d->base + (uintptr_t)off);
}

pcie_status_t pcie_link_train(pcie_dev_t *dev, uint32_t timeout_cycles)
{
    if ((dev == NULL) || (timeout_cycles == 0u)) {
        return PCIE_EINVAL;
    }

    /* Ensure core is ready */
    if (!pcie_core_is_ready(dev)) {
        return PCIE_EBUSY;
    }

    /* Enable LTSSM */
    hal_mmio_rmw32(reg(dev, PCIE_REG_CMD), PCIE_CMD_LTSSM_EN, 0u);
    hal_mmio_rmw32(reg(dev, PCIE_REG_LTSSM_CTRL), PCIE_LTSSM_CTRL_START, 0u);

    for (uint32_t i = 0u; i < timeout_cycles; ++i) {
        uint32_t st = hal_mmio_read32(reg(dev, PCIE_REG_LTSSM_STATUS));
        if ((st & PCIE_LTSSM_STATUS_LINK_UP) != 0u) {
            return PCIE_OK;
        }
    }
    return PCIE_ETIMEOUT;
}

bool pcie_link_is_up(pcie_dev_t *dev)
{
    if (dev == NULL) {
        return false;
    }
    uint32_t st = hal_mmio_read32(reg(dev, PCIE_REG_LTSSM_STATUS));
    return (st & PCIE_LTSSM_STATUS_LINK_UP) != 0u;
}

uint32_t pcie_link_ltssm_status(pcie_dev_t *dev)
{
    if (dev == NULL) {
        return 0u;
    }
    return hal_mmio_read32(reg(dev, PCIE_REG_LTSSM_STATUS));
}
