#include "pcie_cfg.h"
#include "../../hal/hal_mmio.h"
#include "../../include/pcie_regs.h"

static inline uintptr_t reg(pcie_dev_t *d, uint32_t off)
{
    return (d->base + (uintptr_t)off);
}

static uint32_t compose_cfg_addr(uint8_t bus, uint8_t dev, uint8_t fun, uint16_t reg, uint8_t byte)
{
    uint32_t addr = 0u;
    addr |= ((uint32_t)bus << PCIE_CFG_ADDR_BUS_SHIFT);
    addr |= ((uint32_t)dev << PCIE_CFG_ADDR_DEV_SHIFT);
    addr |= ((uint32_t)fun << PCIE_CFG_ADDR_FUN_SHIFT);
    addr |= (((uint32_t)reg & 0xFCu) << (PCIE_CFG_ADDR_REG_SHIFT - 2u));
    addr |= ((uint32_t)(reg & 0x3u) + (uint32_t)byte) & 0x3u; /* byte offset */
    return addr;
}

pcie_status_t pcie_cfg_read(pcie_dev_t *dev,
                            uint8_t bus, uint8_t device, uint8_t function,
                            uint16_t regn, pcie_cfg_width_t width, uint32_t *out_val)
{
    if ((dev == NULL) || (out_val == NULL)) {
        return PCIE_EINVAL;
    }
    if ((width != PCIE_CFG_WIDTH_8) && (width != PCIE_CFG_WIDTH_16) && (width != PCIE_CFG_WIDTH_32)) {
        return PCIE_EINVAL;
    }

    uint8_t byte = 0u;
    uint32_t addr = compose_cfg_addr(bus, device, function, regn, 0u);
    hal_mmio_write32(reg(dev, PCIE_REG_CFG_ADDR), addr);
    uint32_t data = hal_mmio_read32(reg(dev, PCIE_REG_CFG_DATA));

    switch (width) {
    case PCIE_CFG_WIDTH_8:
        byte = (uint8_t)(regn & 0x3u);
        *out_val = (data >> (8u * byte)) & 0xFFu;
        break;
    case PCIE_CFG_WIDTH_16:
        byte = (uint8_t)((regn & 0x2u));
        *out_val = (data >> (8u * byte)) & 0xFFFFu;
        break;
    case PCIE_CFG_WIDTH_32:
    default:
        *out_val = data;
        break;
    }
    return PCIE_OK;
}

pcie_status_t pcie_cfg_write(pcie_dev_t *dev,
                             uint8_t bus, uint8_t device, uint8_t function,
                             uint16_t regn, pcie_cfg_width_t width, uint32_t value)
{
    if (dev == NULL) {
        return PCIE_EINVAL;
    }
    if ((width != PCIE_CFG_WIDTH_8) && (width != PCIE_CFG_WIDTH_16) && (width != PCIE_CFG_WIDTH_32)) {
        return PCIE_EINVAL;
    }

    uint32_t data = 0u;
    uint32_t addr = compose_cfg_addr(bus, device, function, regn, 0u);

    if (width == PCIE_CFG_WIDTH_32) {
        data = value;
    } else {
        /* Read-modify-write of the containing DWORD */
        hal_mmio_write32(reg(dev, PCIE_REG_CFG_ADDR), addr);
        data = hal_mmio_read32(reg(dev, PCIE_REG_CFG_DATA));
        uint8_t shift = (uint8_t)(8u * (regn & 0x3u));
        uint32_t mask = (width == PCIE_CFG_WIDTH_16) ? 0xFFFFu : 0xFFu;
        data &= ~(mask << shift);
        data |= ((value & mask) << shift);
    }

    hal_mmio_write32(reg(dev, PCIE_REG_CFG_ADDR), addr);
    hal_mmio_write32(reg(dev, PCIE_REG_CFG_DATA), data);
    return PCIE_OK;
}
