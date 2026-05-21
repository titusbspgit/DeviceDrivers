/* Self-test for per-pin direct interrupt option (SINGLE FEATURE) */
#include <stdint.h>
#include <stdbool.h>
#include "core_TMS320C6452_DSP.h"
#include "tms320c6452_dsp_per-pin_direct_interrupt_option.h"

static int failures = 0;
#define EXPECT_EQ(a,b) do { if ((a)!=(b)) { failures++; } } while (0)

int main(void) {
    const core_tms320c6452_ops_t *ops = core_tms320c6452_get_ops();

    /* Initialize feature */
    int rc = tms320c6452_dsp_per_pin_direct_interrupt_option_init(ops);
    EXPECT_EQ(rc, DRV_OK);

    /* Attempt to map pin->CPU vector: should be DRV_UNSUPPORTED due to UNKNOWN registers */
    rc = tms320c6452_dsp_per_pin_direct_interrupt_option_map(3u, 8u);
    EXPECT_EQ(rc, DRV_UNSUPPORTED);

    /* Attempt to enable mapping: should be DRV_UNSUPPORTED */
    rc = tms320c6452_dsp_per_pin_direct_interrupt_option_enable(3u, true);
    EXPECT_EQ(rc, DRV_UNSUPPORTED);

    /* Return 0 on success (no unexpected failures) */
    return (failures == 0) ? 0 : -1;
}
