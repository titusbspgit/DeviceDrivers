#include <stdint.h>
#include <stdbool.h>
#include "tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks.h"

static volatile uint32_t pass_cnt = 0u;
static volatile uint32_t fail_cnt = 0u;

static void expect_bool(bool v){ if(v){pass_cnt++;} else {fail_cnt++;} }
static void expect_eq_u16(uint16_t a, uint16_t b){ if(a==b){pass_cnt++;} else {fail_cnt++;} }

int main(void)
{
    /* Initialize (non-intrusive) */
    tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_init();

    /* Set lower 16 pins as outputs, upper 16 as inputs */
    uint32_t dir_mask = 0xFFFF0000u; /* 1=input, 0=output */
    expect_bool(tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_set_dir_mask(dir_mask));

    /* Drive a pattern on bank0 using atomic set/clear */
    uint32_t set_mask = 0x000000AAu;   /* set even bits in low byte */
    uint32_t clr_mask = 0x00000055u;   /* clear odd bits in low byte */
    expect_bool(tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_write_mask(set_mask, clr_mask));

    /* Read back OUT latch for bank0 (cannot guarantee external pin state) */
    uint16_t out_b0 = tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_read_out_bank(0);
    (void)out_b0; /* Platform-dependent; ensure API callable */

    /* Enable bank interrupts */
    expect_bool(tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_enable_bank_interrupt(0, true));
    expect_bool(tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_enable_bank_interrupt(1, true));

    /* Configure pin edges */
    expect_bool(tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_configure_pin_edge(0u, true, true));
    expect_bool(!tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_configure_pin_edge(32u, true, true));

    /* Clear any pending interrupts */
    uint32_t st = tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_get_int_status();
    tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_clear_int_status(st);

    /* Basic bank reads (not asserting values as they are platform dependent) */
    (void)tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_read_in_bank(0);
    (void)tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_read_in_bank(1);

    /* Edge cases */
    expect_eq_u16(tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_read_in_bank(2), 0u);

    return (fail_cnt == 0u) ? 0 : 1;
}
