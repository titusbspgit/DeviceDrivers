#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../include/feature_in.h"

static void delay_cycles(volatile uint32_t n){ while(n--){ __asm__ __volatile__("" ::: "memory"); }}

int main(void)
{
    (void)gpio_init(TMS320C6452_GPIO_BASE_ADDR);

    (void)gpio_set_direction(2u, GPIO_DIR_OUTPUT);

    (void)gpio_write_pin(2u, true);
    delay_cycles(1000u);

    bool v=false;
    (void)gpio_read_pin(2u, &v);

    printf("in: p2=%u\n", (unsigned)v);
    bool pass = (v == true);
    printf("%s\n", pass ? "PASS" : "FAIL");
    return pass ? 0 : 1;
}
