#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "TMS320C6452 DSP_gpio_banked_arch_32x2.h"

/* Mock MMIO space for unit-like testing (no hardware dependency) */
static uint32_t mock_mem[0x100u / 4u];
static const uintptr_t mock_base = 0x1000u;

static uint32_t addr_to_idx(uintptr_t addr)
{
    uintptr_t off = (addr - mock_base) & 0xFFu;
    return (uint32_t)(off >> 2);
}

static uint32_t mock_read32(uintptr_t addr)
{
    return mock_mem[addr_to_idx(addr)];
}

static void mock_write32(uintptr_t addr, uint32_t val)
{
    uint32_t idx = addr_to_idx(addr);
    uintptr_t off = (addr - mock_base) & 0xFFu;

    if (off == GPIO_REG_INTSTAT01_OFFSET) {
        /* W1C behavior: write '1' clears bits */
        mock_mem[idx] &= ~val;
    } else if (off == GPIO_REG_SET_DATA01_OFFSET) {
        mock_mem[addr_to_idx(mock_base + GPIO_REG_OUT_DATA01_OFFSET)] |= val;
    } else if (off == GPIO_REG_CLR_DATA01_OFFSET) {
        mock_mem[addr_to_idx(mock_base + GPIO_REG_OUT_DATA01_OFFSET)] &= ~val;
    } else {
        mock_mem[idx] = val;
    }
}

static void mock_bitset32(uintptr_t addr, uint32_t mask)
{
    mock_mem[addr_to_idx(addr)] |= mask;
}

static void mock_bitclear32(uintptr_t addr, uint32_t mask)
{
    mock_mem[addr_to_idx(addr)] &= ~mask;
}

static const struct gpio_core_ops mock_ops = {
    .read32 = mock_read32,
    .write32 = mock_write32,
    .bitset32 = mock_bitset32,
    .bitclear32 = mock_bitclear32,
    .base = (uintptr_t)0x1000u,
    .is_big_endian = true
};

static void reset_mock(void)
{
    for (uint32_t i = 0u; i < (uint32_t)(sizeof(mock_mem) / sizeof(mock_mem[0])); ++i) {
        mock_mem[i] = 0u;
    }
}

int main(void)
{
    int32_t rc;
    uint32_t val;

    reset_mock();

    rc = gpio_arch_init(&mock_ops);
    if (rc != GPIO_EOK) { printf("init failed\n"); return 1; }

    /* Configure GP[1] (bank0 bit1) and GP[20] (bank1 bit4) as outputs */
    rc = gpio_set_dir_bank(0u, (1u << 1), true);
    rc |= gpio_set_dir_bank(1u, (1u << 4), true);
    if (rc != GPIO_EOK) { printf("set_dir failed\n"); return 1; }

    /* Drive them high then low */
    rc = gpio_write_bank(0u, (1u << 1), 0u);
    rc |= gpio_write_bank(1u, (1u << 4), 0u);
    if (rc != GPIO_EOK) { printf("write set failed\n"); return 1; }

    rc = gpio_write_bank(0u, 0u, (1u << 1));
    rc |= gpio_write_bank(1u, 0u, (1u << 4));
    if (rc != GPIO_EOK) { printf("write clr failed\n"); return 1; }

    /* Mock input: set IN_DATA01 to indicate bits */
    mock_write32(mock_base + GPIO_REG_IN_DATA01_OFFSET, 0x00020010u); /* bank0 bit1, bank1 bit4 */

    val = 0u;
    rc = gpio_read_bank(0u, &val);
    if (rc != GPIO_EOK || val != (1u << 1)) { printf("read b0 failed %d %u\n", (int)rc, val); return 1; }

    val = 0u;
    rc = gpio_read_bank(1u, &val);
    if (rc != GPIO_EOK || val != (1u << 4)) { printf("read b1 failed %d %u\n", (int)rc, val); return 1; }

    /* Enable interrupts for those pins */
    rc = gpio_irq_enable_bank(0u, (1u << 1), true);
    rc |= gpio_irq_enable_bank(1u, (1u << 4), true);
    if (rc != GPIO_EOK) { printf("irq enable failed\n"); return 1; }

    /* Mock interrupt status set */
    mock_mem[addr_to_idx(mock_base + GPIO_REG_INTSTAT01_OFFSET)] = 0x00020010u;

    val = 0u;
    rc = gpio_irq_status_bank(0u, &val);
    if (rc != GPIO_EOK || val != (1u << 1)) { printf("irq st b0 failed %d %u\n", (int)rc, val); return 1; }

    val = 0u;
    rc = gpio_irq_status_bank(1u, &val);
    if (rc != GPIO_EOK || val != (1u << 4)) { printf("irq st b1 failed %d %u\n", (int)rc, val); return 1; }

    /* Edge cases */
    rc = gpio_set_dir_bank(2u, 1u, true);
    if (rc == GPIO_EOK) { printf("invalid bank accepted\n"); return 1; }

    rc = gpio_read_bank(0u, (uint32_t*)0);
    if (rc == GPIO_EOK) { printf("null ptr accepted\n"); return 1; }

    printf("OK\n");
    return 0;
}
