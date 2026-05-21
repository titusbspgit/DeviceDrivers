#include <stdint.h>
#include <stdbool.h>
#include "tms320c6452_gpio_pin_multiplexing_control.h"

/* NOTE: PINMUX register addresses/fields are device-specific and UNKNOWN here.
 * This test uses a simulated memory-mapped register to demonstrate deterministic behavior. */

static volatile uint32_t SIM_PINMUX0;

int main(void)
{
    /* Example: field is 2 bits wide at LSBs, selecting 0b01 for GPIO. */
    tms320c6452_pinmux_entry_t tbl[] = {
        { &SIM_PINMUX0, 0x3u, 0x1u },
    };

    /* Clear to non-GPIO first */
    SIM_PINMUX0 = 0x0u;

    /* Apply GPIO selection */
    tms320c6452_pinmux_apply(tbl, (uint32_t)(sizeof(tbl)/sizeof(tbl[0])));

    /* Verify */
    bool ok = tms320c6452_pinmux_verify(tbl, (uint32_t)(sizeof(tbl)/sizeof(tbl[0])));

    /* Simple pass/fail loop (no stdio). */
    for (;;) {
        if (!ok) {
            /* trap here for debug */
        }
    }

    return 0;
}
