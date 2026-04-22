/*
 * Test: Direction control (DIR01)
 */
#include <stdio.h>
#include "tms320c6452_dsp_direction_control_(dir01).h"

int main(void)
{
    gpio_drv_status_t st;
    uint32_t is_in;

    st = gpio_dir_init();
    printf("gpio_dir_init: %d\n", (int)st);

    st = gpio_set_pin_output(5u);
    printf("gpio_set_pin_output(5): %d (expected unknown_reg)\n", (int)st);

    st = gpio_get_pin_is_input(5u, &is_in);
    printf("gpio_get_pin_is_input(5): %d, shadow_is_input=%u\n", (int)st, is_in);

    st = gpio_set_pin_input(5u);
    printf("gpio_set_pin_input(5): %d (expected unknown_reg)\n", (int)st);

    st = gpio_get_pin_is_input(5u, &is_in);
    printf("gpio_get_pin_is_input(5): %d, shadow_is_input=%u\n", (int)st, is_in);

    return 0;
}
