#include <stdint.h>
#include <stdbool.h>
#include "TMS320C6452 DSP_GPIO banked architecture (32 signals, 2 banks).h"

static volatile uint32_t pass_cnt = 0u;
static volatile uint32_t fail_cnt = 0u;

static void expect_bool(bool v){ if(v){pass_cnt++;} else {fail_cnt++;} }
static void expect_eq_u16(uint16_t a, uint16_t b){ if(a==b){pass_cnt++;} else {fail_cnt++;} }

int main(void)
{
    TMS320C6452_DSP_GPIO_banked_architecture_32_signals_2_banks_init();

    uint32_t dir_mask = 0xFFFF0000u;
    expect_bool(TMS320C6452_DSP_GPIO_banked_architecture_32_signals_2_banks_set_dir_mask(dir_mask));

    uint32_t set_mask = 0x000000AAu;
    uint32_t clr_mask = 0x00000055u;
    expect_bool(TMS320C6452_DSP_GPIO_banked_architecture_32_signals_2_banks_write_mask(set_mask, clr_mask));

    (void)TMS320C6452_DSP_GPIO_banked_architecture_32_signals_2_banks_read_out_bank(0);

    expect_bool(TMS320C6452_DSP_GPIO_banked_architecture_32_signals_2_banks_enable_bank_interrupt(0, true));
    expect_bool(TMS320C6452_DSP_GPIO_banked_architecture_32_signals_2_banks_enable_bank_interrupt(1, true));

    expect_bool(TMS320C6452_DSP_GPIO_banked_architecture_32_signals_2_banks_configure_pin_edge(0u, true, true));
    expect_bool(!TMS320C6452_DSP_GPIO_banked_architecture_32_signals_2_banks_configure_pin_edge(32u, true, true));

    uint32_t st = TMS320C6452_DSP_GPIO_banked_architecture_32_signals_2_banks_get_int_status();
    TMS320C6452_DSP_GPIO_banked_architecture_32_signals_2_banks_clear_int_status(st);

    (void)TMS320C6452_DSP_GPIO_banked_architecture_32_signals_2_banks_read_in_bank(0);
    (void)TMS320C6452_DSP_GPIO_banked_architecture_32_signals_2_banks_read_in_bank(1);

    expect_eq_u16(TMS320C6452_DSP_GPIO_banked_architecture_32_signals_2_banks_read_in_bank(2), 0u);

    return (fail_cnt == 0u) ? 0 : 1;
}
