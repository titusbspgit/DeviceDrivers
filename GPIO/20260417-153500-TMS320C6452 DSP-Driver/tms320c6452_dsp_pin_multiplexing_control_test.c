#include <stdint.h>
#include <stdbool.h>
#include "tms320c6452_dsp_pin_multiplexing_control.h"

static volatile uint32_t pass_cnt = 0u;
static volatile uint32_t fail_cnt = 0u;

static void expect_bool(bool c){ if(c) pass_cnt++; else fail_cnt++; }
static void expect_int(int a,int b){ if(a==b) pass_cnt++; else fail_cnt++; }

int main(void)
{
    expect_bool(tms320c6452_dsp_pin_multiplexing_control_validate(0u));
    expect_bool(tms320c6452_dsp_pin_multiplexing_control_validate(31u));
    expect_bool(!tms320c6452_dsp_pin_multiplexing_control_validate(32u));

    expect_bool(!tms320c6452_dsp_pin_multiplexing_control_configure_gpio(0u));
    expect_int(tms320c6452_dsp_pin_multiplexing_control_query_function(0u), -1);

    while(1){ }
    return 0;
}
