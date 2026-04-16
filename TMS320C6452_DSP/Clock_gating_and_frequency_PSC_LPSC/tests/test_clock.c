#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../include/feature_clock.h"

/* Override weak HAL for test to simulate clock enable success */
bool gpio_hal_enable_clock(void)
{
    return true;
}

int main(void)
{
    gpio_status_t st = gpio_init(TMS320C6452_GPIO_BASE_ADDR);
    printf("gpio_init status=%u (0=OK)\n", (unsigned)st);
    printf("%s\n", (st == GPIO_STATUS_OK) ? "PASS" : "FAIL");
    return (st == GPIO_STATUS_OK) ? 0 : 1;
}
