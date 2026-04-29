/*
 * Test: GPIO banked architecture (32 signals, 2 banks)
 * Baremetal self-check of bank mapping helpers.
 */
#include <stdint.h>
#include <stdbool.h>

#include "tms320c6452_dsp_gpio_banked_architecture_(32_signals,_2_banks).h"

/* Optional observable variables for simple baremetal validation */
static volatile uint32_t g_test_fail_count = 0U;
static volatile uint32_t g_test_pass = 0U;

static void test_check_mapping(void)
{
    for (uint32_t pin = 0U; pin < TMS320C6452_DSP_GPIO_FEATURE_MAX_PINS; ++pin)
    {
        const uint32_t exp_bank = (pin >> 4) & 0x1U;
        const uint32_t exp_mask = (uint32_t)(1UL << (pin & 0xFU));

        const uint32_t bank = tms320c6452_dsp_gpio_bank_index_from_pin(pin);
        const uint32_t mask = tms320c6452_dsp_gpio_bitmask_from_pin(pin);

        if ((bank != exp_bank) || (mask != exp_mask))
        {
            g_test_fail_count++;
        }
    }
}

static void test_bank_helpers(void)
{
    /* Try calling bank helpers; accept either OK or UNSUPPORTED for this feature-only test */
    uint32_t val = 0U;
    int st;

    st = tms320c6452_dsp_gpio_bank_read_in(0U, &val);
    if (!((st == (int)TMS320C6452_DSP_GPIO_FEAT_OK) || (st == (int)TMS320C6452_DSP_GPIO_FEAT_ERR_UNSUPPORTED))) { g_test_fail_count++; }

    st = tms320c6452_dsp_gpio_bank_read_out(1U, &val);
    if (!((st == (int)TMS320C6452_DSP_GPIO_FEAT_OK) || (st == (int)TMS320C6452_DSP_GPIO_FEAT_ERR_UNSUPPORTED))) { g_test_fail_count++; }

    st = tms320c6452_dsp_gpio_bank_set_mask(0U, 0x0001U);
    if (!((st == (int)TMS320C6452_DSP_GPIO_FEAT_OK) || (st == (int)TMS320C6452_DSP_GPIO_FEAT_ERR_UNSUPPORTED))) { g_test_fail_count++; }

    st = tms320c6452_dsp_gpio_bank_clear_mask(1U, 0x8000U);
    if (!((st == (int)TMS320C6452_DSP_GPIO_FEAT_OK) || (st == (int)TMS320C6452_DSP_GPIO_FEAT_ERR_UNSUPPORTED))) { g_test_fail_count++; }
}

int tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_test(void)
{
    (void)tms320c6452_dsp_gpio_arch_init();
    test_check_mapping();
    test_bank_helpers();

    if (g_test_fail_count == 0U)
    {
        g_test_pass = 1U;
        return 0;
    }
    else
    {
        g_test_pass = 0U;
        return -1;
    }
}

#ifdef UNIT_TEST
int main(void)
{
    return tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_test();
}
#endif
