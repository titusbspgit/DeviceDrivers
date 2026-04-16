#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../include/feature_pinmux.h"

/* Override weak HAL for test to simulate success */
bool gpio_hal_configure_pinmux(uint32_t pin)
{
    (void)pin;
    return true; /* Assume pinmux success */
}

int main(void)
{
    (void)gpio_init(TMS320C6452_GPIO_BASE_ADDR);
    bool ok0 = feature_configure_pin_as_gpio(0u);
    bool ok1 = feature_configure_pin_as_gpio(16u);
    printf("Pinmux cfg 0=%u 16=%u\n", (unsigned)ok0, (unsigned)ok1);
    printf("%s\n", (ok0 && ok1) ? "PASS" : "FAIL");
    return (ok0 && ok1) ? 0 : 1;
}
