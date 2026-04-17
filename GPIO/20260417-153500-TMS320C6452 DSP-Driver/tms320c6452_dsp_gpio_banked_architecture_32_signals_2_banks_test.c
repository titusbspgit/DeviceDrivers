#include <stdint.h>
#include <stdbool.h>
#include "tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks.h"

#ifndef TEST_PIN
#define TEST_PIN (0u)
#endif

static volatile uint32_t pass_cnt = 0u;
static volatile uint32_t fail_cnt = 0u;

static void expect_bool(bool c){ if(c) pass_cnt++; else fail_cnt++; }
static void expect_int(int a,int b){ if(a==b) pass_cnt++; else fail_cnt++; }

int main(void)
{
    tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_init();

    expect_bool(tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_set_direction(TEST_PIN,false));
    expect_bool(tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_write(TEST_PIN,false));
    expect_int(tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_read_out(TEST_PIN),0);

    expect_bool(tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_write(TEST_PIN,true));
    expect_int(tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_read_out(TEST_PIN),1);

    /* Toggle */
    int before = tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_read_out(TEST_PIN);
    expect_bool(tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_toggle(TEST_PIN));
    int after = tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_read_out(TEST_PIN);
    expect_bool(before != after);

    /* IRQ path */
    expect_bool(tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_irq_enable(TEST_PIN,true,false));
    (void)tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_irq_status();
    tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_irq_clear(0u);
    expect_bool(tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_irq_disable(TEST_PIN));

    /* Bounds */
    expect_bool(!tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_set_direction(32u,false));

    while(1){ }
    return 0;
}
