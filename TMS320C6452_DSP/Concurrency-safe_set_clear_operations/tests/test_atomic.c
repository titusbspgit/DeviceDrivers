#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../include/feature_atomic.h"

int main(void)
{
    (void)gpio_init(TMS320C6452_GPIO_BASE_ADDR);
    (void)gpio_set_direction(6u, GPIO_DIR_OUTPUT);
    (void)gpio_set_direction(7u, GPIO_DIR_OUTPUT);

    (void)gpio_write_mask(GPIO_PIN_MASK(6u), GPIO_PIN_MASK(7u)); /* set 6, clear 7 atomically */

    bool v6=false, v7=true;
    (void)gpio_read_pin(6u, &v6);
    (void)gpio_read_pin(7u, &v7);

    printf("atomic: p6=%u p7=%u\n", (unsigned)v6, (unsigned)v7);
    printf("%s\n", (v6==true && v7==false) ? "PASS" : "FAIL");
    return (v6==true && v7==false) ? 0 : 1;
}
