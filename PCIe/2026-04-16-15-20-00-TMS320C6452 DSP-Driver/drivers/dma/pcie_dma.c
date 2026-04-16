#include "pcie_dma.h"
#include "../../hal/hal_mmio.h"
#include "../../include/pcie_regs.h"

static inline uintptr_t reg(pcie_dev_t *d, uint32_t off)
{
    return (d->base + (uintptr_t)off);
}

pcie_status_t pcie_dma_start_memcpy(pcie_dev_t *dev, uint32_t src, uint32_t dst, uint32_t len_bytes)
{
    if ((dev == NULL) || (len_bytes == 0u)) {
        return PCIE_EINVAL;
    }
    /* TODO: Confirm address widths and alignment requirements */
    hal_mmio_write32(reg(dev, PCIE_REG_DMA_SRC), src);
    hal_mmio_write32(reg(dev, PCIE_REG_DMA_DST), dst);
    hal_mmio_write32(reg(dev, PCIE_REG_DMA_LEN), len_bytes);
    hal_mmio_rmw32(reg(dev, PCIE_REG_DMA_CTRL), PCIE_DMA_CTRL_START, 0u);
    return PCIE_OK;
}

bool pcie_dma_is_busy(pcie_dev_t *dev)
{
    if (dev == NULL) {
        return false;
    }
    uint32_t v = hal_mmio_read32(reg(dev, PCIE_REG_DMA_CTRL));
    return (v & PCIE_DMA_CTRL_BUSY) != 0u;
}
