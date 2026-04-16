#include "../include/Clock_gating_and_frequency_PSC_LPSC_api.h"

/* Simulate clock enable success via HAL */
bool gpio_hal_enable_clock(void) __attribute__((weak));
bool gpio_hal_enable_clock(void){ return true; }

int Clock_gating_and_frequency_PSC_LPSC_feature_run(void)
{
    return (gpio_init(TMS320C6452_GPIO_BASE_ADDR) == GPIO_STATUS_OK) ? 0 : -1;
}
