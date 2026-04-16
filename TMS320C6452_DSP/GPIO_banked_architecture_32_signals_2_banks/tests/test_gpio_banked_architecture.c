#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../include/feature_gpio_banked_architecture.h"

static void delay_cycles(volatile uint32_t n)
{
    while (n-- != 0u) { __asm__ __volatile__("" ::: "memory"); }
}

int main(void)
{
    printf("Feature: GPIO banked architecture (32 signals, 2 banks)\n");

    if (gpio_init(TMS320C6452_GPIO_BASE_ADDR) != GPIO_STATUS_OK)
    {
        printf("Init failed\n");
        return 1;
    }

    /* Configure one pin in each bank as output */
    (void)gpio_set_direction(0u, GPIO_DIR_OUTPUT);
    (void)gpio_set_direction(16u, GPIO_DIR_OUTPUT);

    /* Drive different states to ensure bit mapping doesn't alias between banks */
    (void)gpio_write_pin(0u, true);
    (void)gpio_write_pin(16u, false);
    delay_cycles(1000u);

    bool v0=false, v16=true;
    (void)gpio_read_pin(0u, &v0);
    (void)gpio_read_pin(16u, &v16);

    printf("Bank0 pin0=%u, Bank1 pin16=%u\n", (unsigned)v0, (unsigned)v16);
    bool pass = (v0 == true) && (v16 == false);

    printf("%s\n", pass ? "PASS" : "FAIL");
    return pass ? 0 : 2;
}
