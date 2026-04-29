#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "TMS320C6452 DSP_gpio_banked_arch_32x2.h"

/* Minimal mock core ops for host build testing (no hardware). */
static uint32_t mock_mem[0x100/4u];

static uint32_t mock_read32(uintptr_t addr)
{
    uintptr_t off = (addr & 0xFFu);
    return mock_mem[off >> 2];
}

static void mock_write32(uintptr_t addr, uint32_t val)
{
    uintptr_t off = (addr & 0xFFu);
    mock_mem[off >> 2] = val;
}

static void mock_bitset32(uintptr_t addr, uint32_t mask)
{
    uintptr_t off = (addr & 0xFFu);
    mock_mem[off >> 2] |= mask;
}

static void mock_bitclear32(uintptr_t addr, uint32_t mask)
{
    uintptr_t off = (addr & 0xFFu);
    mock_mem[off >> 2] &= ~mask;
}

static const struct gpio_core_ops mock_ops = {
    .read32 = mock_read32,
    .write32 = mock_write32,
    .bitset32 = mock_bitset32,
    .bitclear32 = mock_bitclear32,
    .base = 0x0u,
    .is_big_endian = true
};

static void reset_mock(void)
{
    for (unsigned i = 0u; i < (sizeof(mock_mem)/sizeof(mock_mem[0])); ++i) {
        mock_mem[i] = 0u;
    }
}

int main(void)
{
    int32_t rc;
    uint32_t val;

    reset_mock();

    rc = gpio_arch_init(&mock_ops);
    if (rc != 0) { printf("init failed\n"); return 1; }

    /* Configure GP[1] (bank0 bit1) and GP[20] (bank1 bit4) as outputs */
    rc = gpio_set_dir_bank(0u, (1u << 1), true);
    rc |= gpio_set_dir_bank(1u, (1u << 4), true);
    if (rc != 0) { printf("set_dir failed\n"); return 1; }

    /* Drive them high then low */
    rc = gpio_write_bank(0u, (1u << 1), 0u);
    rc |= gpio_write_bank(1u, (1u << 4), 0u);
    if (rc != 0) { printf("write set failed\n"); return 1; }

    rc = gpio_write_bank(0u, 0u, (1u << 1));
    rc |= gpio_write_bank(1u, 0u, (1u << 4));
    if (rc != 0) { printf("write clr failed\n"); return 1; }

    /* Mock input: set IN_DATA01 to indicate bits */
    mock_write32(GPIO_REG_IN_DATA01_OFFSET, 0x00020010u); /* bank0 bit1, bank1 bit4 */

    val = 0u;
    rc = gpio_read_bank(0u, &val);
    if (rc != 0 || val != (1u << 1)) { printf("read b0 failed %u %u\n", rc, val); return 1; }

    val = 0u;
    rc = gpio_read_bank(1u, &val);
    if (rc != 0 || val != (1u << 4)) { printf("read b1 failed %u %u\n", rc, val); return 1; }

    /* Enable interrupts for those pins */
    rc = gpio_irq_enable_bank(0u, (1u << 1), true);
    rc |= gpio_irq_enable_bank(1u, (1u << 4), true);
    if (rc != 0) { printf("irq enable failed\n"); return 1; }

    /* Mock interrupt status */
    mock_write32(GPIO_REG_INTSTAT01_OFFSET, 0x00020010u);

    val = 0u;
    rc = gpio_irq_status_bank(0u, &val);
    if (rc != 0 || val != (1u << 1)) { printf("irq st b0 failed %u %u\n", rc, val); return 1; }

    val = 0u;
    rc = gpio_irq_status_bank(1u, &val);
    if (rc != 0 || val != (1u << 4)) { printf("irq st b1 failed %u %u\n", rc, val); return 1; }

    printf("OK\n");
    return 0;
}
