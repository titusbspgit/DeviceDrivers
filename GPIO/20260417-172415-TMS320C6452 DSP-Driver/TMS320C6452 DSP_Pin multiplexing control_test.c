#include <stdint.h>
#include <stdbool.h>
#include "TMS320C6452 DSP_Pin multiplexing control.h"

static volatile uint32_t pass_cnt = 0u;
static volatile uint32_t fail_cnt = 0u;

static void expect_bool(bool v){ if(v){pass_cnt++;} else {fail_cnt++;} }
static void expect_eq_i(int a, int b){ if(a==b){pass_cnt++;} else {fail_cnt++;} }

int main(void)
{
    expect_eq_i(TMS320C6452_DSP_Pin_multiplexing_control_init(), TMS320C6452_DSP_PINMUX_OK);
    expect_bool(!TMS320C6452_DSP_Pin_multiplexing_control_is_ready());

    TMS320C6452_DSP_pinmux_request_t req = { .pin_id = 0u, .function = 0u };
    expect_eq_i(TMS320C6452_DSP_Pin_multiplexing_control_apply(&req), TMS320C6452_DSP_PINMUX_ERR_UNKNOWN);

    uint32_t func = 0xFFFFFFFFu;
    expect_eq_i(TMS320C6452_DSP_Pin_multiplexing_control_readback(0u, &func), TMS320C6452_DSP_PINMUX_ERR_UNKNOWN);

    return (fail_cnt == 0u) ? 0 : 1;
}
