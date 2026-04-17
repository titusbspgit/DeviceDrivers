#include <stdint.h>
#include <stdbool.h>
#include "tms320c6452_dsp_pin_multiplexing_control.h"

static volatile uint32_t pass_cnt = 0u;
static volatile uint32_t fail_cnt = 0u;

static void expect_bool(bool v){ if(v){pass_cnt++;} else {fail_cnt++;} }
static void expect_eq_i(int a, int b){ if(a==b){pass_cnt++;} else {fail_cnt++;} }

int main(void)
{
    /* Initialize driver (no-op) */
    expect_eq_i(tms320c6452_dsp_pin_multiplexing_control_init(), TMS320C6452_DSP_PINMUX_OK);

    /* Should not be ready because register map is UNKNOWN */
    expect_bool(!tms320c6452_dsp_pin_multiplexing_control_is_ready());

    /* Apply request should deterministically return ERR_UNKNOWN */
    tms320c6452_dsp_pinmux_request_t req = { .pin_id = 0u, .function = 0u };
    expect_eq_i(tms320c6452_dsp_pin_multiplexing_control_apply(&req), TMS320C6452_DSP_PINMUX_ERR_UNKNOWN);

    /* Readback should deterministically return ERR_UNKNOWN */
    uint32_t func = 0xFFFFFFFFu;
    expect_eq_i(tms320c6452_dsp_pin_multiplexing_control_readback(0u, &func), TMS320C6452_DSP_PINMUX_ERR_UNKNOWN);

    return (fail_cnt == 0u) ? 0 : 1;
}
