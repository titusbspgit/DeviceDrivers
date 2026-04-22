/*
 * Test: GPIO banked architecture (32 signals, 2 banks)
 */
#include <stdio.h>
#include "tms320c6452_dsp_gpio_banked_architecture_(32_signals,_2_banks).h"

int main(void)
{
    gpio_bank_bit_t bb;
    unsigned errors = 0u;

    for (uint32_t pin = 0u; pin < 32u; ++pin) {
        if (gpio_arch_map_pin(pin, &bb) != GPIO_DRV_OK) {
            ++errors;
            continue;
        }
        uint32_t expect_bank = (pin >> 4) & 0x1u;
        uint32_t expect_bit  = pin & 0xFu;
        if (bb.bank != expect_bank || bb.bit != expect_bit) {
            ++errors;
        }
    }

    printf("banked_architecture mapping test: %s (errors=%u)\n", (errors==0u)?"PASS":"FAIL", errors);
    return (errors == 0u) ? 0 : 1;
}
