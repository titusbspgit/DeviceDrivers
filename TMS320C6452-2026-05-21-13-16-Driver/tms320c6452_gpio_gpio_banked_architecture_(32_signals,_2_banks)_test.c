#include <stdint.h>
#include <stdio.h>
#include "tms320c6452_gpio_gpio_banked_architecture_(32_signals,_2_banks)_drv.h"

static int test_mapping(void)
{
    int rc = 0;
    uint32_t bank, bit;

    if (tms320c6452_gpio_banked_pin_to_bank(0u, &bank, &bit) != TMS320C6452_GPIO_EOK || bank != 0u || bit != 0u) rc++;
    if (tms320c6452_gpio_banked_pin_mask(0u) != (1u << 0)) rc++;

    if (tms320c6452_gpio_banked_pin_to_bank(15u, &bank, &bit) != TMS320C6452_GPIO_EOK || bank != 0u || bit != 15u) rc++;
    if (tms320c6452_gpio_banked_pin_mask(15u) != (1u << 15)) rc++;

    if (tms320c6452_gpio_banked_pin_to_bank(16u, &bank, &bit) != TMS320C6452_GPIO_EOK || bank != 1u || bit != 0u) rc++;
    if (tms320c6452_gpio_banked_pin_mask(16u) != (1u << 0)) rc++;

    if (tms320c6452_gpio_banked_pin_to_bank(31u, &bank, &bit) != TMS320C6452_GPIO_EOK || bank != 1u || bit != 15u) rc++;
    if (tms320c6452_gpio_banked_pin_mask(31u) != (1u << 15)) rc++;

    if (tms320c6452_gpio_banked_is_valid_pin(32u) != TMS320C6452_GPIO_EINVAL) rc++;

    return rc;
}

static int test_init_and_stubbed_regops(void)
{
    tms320c6452_gpio_banked_ctx_t ctx;
    if (tms320c6452_gpio_banked_init(&ctx, 0u /* UNKNOWN base */) != TMS320C6452_GPIO_EOK) return -1;

    uint32_t val = 0xDEADBEEFu;
    if (tms320c6452_gpio_banked_reg_read(&ctx, 0u /* UNKNOWN ofs */, &val) != TMS320C6452_GPIO_EUNSUPPORTED) return -2;
    if (tms320c6452_gpio_banked_reg_write(&ctx, 0u /* UNKNOWN ofs */, 0u) != TMS320C6452_GPIO_EUNSUPPORTED) return -3;
    if (tms320c6452_gpio_banked_reg_update(&ctx, 0u /* UNKNOWN ofs */, 0xFFFFu, 0x1234u) != TMS320C6452_GPIO_EUNSUPPORTED) return -4;

    return 0;
}

int main(void)
{
    int rc = 0;
    rc += test_mapping();
    rc += test_init_and_stubbed_regops();

    /* Simple pass/fail printout for manual observation */
    if (rc == 0) {
        printf("PASS\n");
    } else {
        printf("FAIL:%d\n", rc);
    }
    return (rc == 0) ? 0 : 1;
}

/* GIT_STATUS: This file will be updated below in the assistant output with the latest commit status. */
