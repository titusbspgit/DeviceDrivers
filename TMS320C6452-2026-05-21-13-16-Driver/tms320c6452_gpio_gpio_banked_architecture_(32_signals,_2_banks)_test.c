#include "tms320c6452_gpio_gpio_banked_architecture_(32_signals,_2_banks).h"

/* Test application for single feature: GPIO banked architecture (32 signals, 2 banks) */

static int test_pin_to_bank_and_bit(void)
{
    uint32_t bank, bit;
    int rc;

    rc = tms320c6452_gpio_gpio_banked_architecture_pin_to_bank(0u, &bank, &bit);
    if (rc != TMS320C6452_GPIO_EOK || bank != 0u || bit != 0u) return -10;

    rc = tms320c6452_gpio_gpio_banked_architecture_pin_to_bank(15u, &bank, &bit);
    if (rc != TMS320C6452_GPIO_EOK || bank != 0u || bit != 15u) return -11;

    rc = tms320c6452_gpio_gpio_banked_architecture_pin_to_bank(16u, &bank, &bit);
    if (rc != TMS320C6452_GPIO_EOK || bank != 1u || bit != 0u) return -12;

    rc = tms320c6452_gpio_gpio_banked_architecture_pin_to_bank(31u, &bank, &bit);
    if (rc != TMS320C6452_GPIO_EOK || bank != 1u || bit != 15u) return -13;

    /* Out-of-range */
    rc = tms320c6452_gpio_gpio_banked_architecture_pin_to_bank(32u, &bank, &bit);
    if (rc != TMS320C6452_GPIO_EINVAL) return -14;

    return 0;
}

static int test_pin_to_mask(void)
{
    uint32_t mask;
    int rc;

    rc = tms320c6452_gpio_gpio_banked_architecture_pin_to_mask(0u, &mask);
    if (rc != TMS320C6452_GPIO_EOK || mask != (1u << 0)) return -20;

    rc = tms320c6452_gpio_gpio_banked_architecture_pin_to_mask(31u, &mask);
    if (rc != TMS320C6452_GPIO_EOK || mask != (1u << 31)) return -21;

    rc = tms320c6452_gpio_gpio_banked_architecture_pin_to_mask(32u, &mask);
    if (rc != TMS320C6452_GPIO_EINVAL) return -22;

    return 0;
}

int main(void)
{
    tms320c6452_gpio_gpio_banked_architecture_ctx_t ctx;
    int rc;

    /* Initialize with UNKNOWN base; no MMIO operations will be performed in this test */
    rc = tms320c6452_gpio_gpio_banked_architecture_init(&ctx, (uintptr_t)TMS320C6452_GPIO_BASE);
    if (rc != TMS320C6452_GPIO_EOK) return rc;

    rc = test_pin_to_bank_and_bit();
    if (rc != 0) return rc;

    rc = test_pin_to_mask();
    if (rc != 0) return rc;

    return 0;
}
