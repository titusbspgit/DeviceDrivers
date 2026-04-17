// Baremetal test for GPIO Basic feature
// Sequence: init -> configure -> write -> read -> edge cases

#include <stdint.h>
#include <stdbool.h>
#include "TMS320C6452_DSP_gpio_basic.h"

// Minimal test harness without stdio
static volatile int g_result = 0; // 0 = PASS path until failure

static void expect_eq_int(int a, int b) {
    if (a != b) { g_result = -1; }
}

int main(void)
{
    const TMS320C6452_DSP_CoreAPI *core = TMS320C6452_DSP_core_get();

    // Init
    int rc = TMS320C6452_DSP_gpio_basic_init(core);
    expect_eq_int(rc, GPIO_BASIC_OK);

    // Configure pin 0 as output (will return UNKNOWN_REG due to missing RAG offsets)
    rc = TMS320C6452_DSP_gpio_basic_configure(core, 0u, GPIO_DIR_OUTPUT);
    // When registers are UNKNOWN, driver returns GPIO_BASIC_ERR_UNKNOWN_REG deterministically
    expect_eq_int(rc, GPIO_BASIC_ERR_UNKNOWN_REG);

    // Write pin 0 (also UNKNOWN)
    rc = TMS320C6452_DSP_gpio_basic_write(core, 0u, true);
    expect_eq_int(rc, GPIO_BASIC_ERR_UNKNOWN_REG);

    // Read pin 0 (also UNKNOWN)
    bool val = false;
    rc = TMS320C6452_DSP_gpio_basic_read(core, 0u, &val);
    expect_eq_int(rc, GPIO_BASIC_ERR_UNKNOWN_REG);

    // Interrupt enable/clear (UNKNOWN)
    rc = TMS320C6452_DSP_gpio_basic_int_enable(core, 0u, true);
    expect_eq_int(rc, GPIO_BASIC_ERR_UNKNOWN_REG);

    rc = TMS320C6452_DSP_gpio_basic_int_clear(core, 0u);
    expect_eq_int(rc, GPIO_BASIC_ERR_UNKNOWN_REG);

    // Edge cases: bad pin
    rc = TMS320C6452_DSP_gpio_basic_configure(core, 33u, GPIO_DIR_OUTPUT);
    expect_eq_int(rc, GPIO_BASIC_ERR_BAD_PIN);

    // Final result in g_result (0 means all expected paths observed)
    return g_result;
}
