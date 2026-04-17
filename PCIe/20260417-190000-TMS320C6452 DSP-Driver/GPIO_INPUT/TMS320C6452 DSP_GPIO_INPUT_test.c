/*
 * TMS320C6452 DSP GPIO INPUT feature - Test Application
 * Validates: init, configure-as-input, and read API paths
 *
 * Notes:
 * - On real hardware, IN_DATA01 reflects the live pin state. This test cannot
 *   force a pin level without external stimulus; it verifies configuration and
 *   performs a non-failing read to demonstrate API usage.
 */

#include <stdio.h>
#include <stdint.h>
#include "TMS320C6452 DSP_GPIO_INPUT.h"

static void print_result(const char *name, int pass)
{
    (void)printf("[GPIO_INPUT][%s]: %s\n", name, pass ? "PASS" : "FAIL");
}

int main(void)
{
    /* Select a representative pin to test; choose GP7 (bit 7) */
    const uint32_t test_pin = 7UL;

    gpio_input_init();

    /* 1) Configure as input with pull-up */
    gpio_status_t st = gpio_input_configure(test_pin, GPIO_PULL_UP);
    print_result("configure_input_pullup", (st == GPIO_STATUS_OK));

    /* 2) Validate DIR bit is set (1 = input) */
    uint32_t dir = gpio_get_dir_reg();
    int dir_ok = ((dir & (1UL << test_pin)) != 0U) ? 1 : 0;
    print_result("dir_bit_set", dir_ok);

    /* 3) Read the pin (value is platform dependent) */
    uint32_t level = 0U;
    st = gpio_input_read(test_pin, &level);
    int read_ok = (st == GPIO_STATUS_OK) ? 1 : 0;
    (void)printf("[GPIO_INPUT][read] pin=%lu level=%lu (platform dependent)\n",
                 (unsigned long)test_pin, (unsigned long)level);
    print_result("read_api", read_ok);

    /* 4) Edge cases: invalid pin and NULL pointer */
    st = gpio_input_read(32UL, &level); /* invalid pin */
    print_result("invalid_pin_read_rejected", (st == GPIO_STATUS_EINVAL));

    st = gpio_input_read(test_pin, (uint32_t *)0); /* null ptr */
    print_result("null_ptr_rejected", (st == GPIO_STATUS_EINVAL));

    (void)printf("GPIO_INPUT test complete.\n");
    return 0;
}
