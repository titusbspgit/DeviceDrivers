/* Test application for GPIO Basic Output (single feature) */
#include <stdint.h>
#include <stdio.h>
#include "tms320c6452_dsp_gpio_basic_output.h"

/* Baremetal stub for printf; replace or remove as needed. */
static void test_log(const char *s) { (void)s; /* UNKNOWN output path */ }

int tms320c6452_dsp_gpio_basic_output_test_main(void)
{
    uint8_t lvl = 0u;
    const uint32_t pin = 0u; /* Test pin (UNKNOWN mapping) */

    if (tms320c6452_dsp_gpio_basic_output_init() != GPIO_BO_OK) {
        test_log("init failed\n");
        return -1;
    }

    if (tms320c6452_dsp_gpio_basic_output_config_pin(pin) != GPIO_BO_OK) {
        test_log("config failed (likely UNKNOWN offsets)\n");
        return -2;
    }

    if (tms320c6452_dsp_gpio_basic_output_write(pin, 1u) != GPIO_BO_OK) {
        test_log("set failed\n");
    }

    (void)tms320c6452_dsp_core_ops.delay_cycles(1000u);

    (void)tms320c6452_dsp_gpio_basic_output_toggle(pin);

    (void)tms320c6452_dsp_core_ops.delay_cycles(1000u);

    if (tms320c6452_dsp_gpio_basic_output_read(pin, &lvl) == GPIO_BO_OK) {
        (void)lvl; /* In baremetal, no I/O to print. */
    }

    tms320c6452_dsp_gpio_basic_output_deinit();
    return 0;
}
