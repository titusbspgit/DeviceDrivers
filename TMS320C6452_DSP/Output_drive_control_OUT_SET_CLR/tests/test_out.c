#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../include/feature_out.h"

static void delay_cycles(volatile uint32_t n){ while(n--){ __asm__ __volatile__("" ::: "memory"); }}

int main(void)
{
    (void)gpio_init(TMS320C6452_GPIO_BASE_ADDR);

    (void)gpio_set_direction(0u, GPIO_DIR_OUTPUT);
    (void)gpio_set_direction(1u, GPIO_DIR_OUTPUT);

    (void)gpio_write_pin(0u, false);
    (void)gpio_write_pin(1u, true);
    delay_cycles(1000u);

    bool v0=false, v1=false;
    (void)gpio_read_pin(0u, &v0);
    (void)gpio_read_pin(1u, &v1);

    printf("out: p0=%u p1=%u\n", (unsigned)v0, (unsigned)v1);
    bool pass = (v0 == false) && (v1 == true);
    printf("%s\n", pass ? "PASS" : "FAIL");
    return pass ? 0 : 1;
}
