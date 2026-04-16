#include "../include/Pin_multiplexing_control_api.h"

/* Override pinmux HAL for this test to simulate success */
bool gpio_hal_configure_pinmux(uint32_t pin) __attribute__((weak));
bool gpio_hal_configure_pinmux(uint32_t pin){ (void)pin; return true; }

int Pin_multiplexing_control_feature_run(void)
{
    if (gpio_init(TMS320C6452_GPIO_BASE_ADDR) != GPIO_STATUS_OK) { return -1; }
    /* Call direction API that internally invokes pinmux hook. If hook fails, we should fail. */
    if (gpio_set_direction(0u, GPIO_DIR_OUTPUT) != GPIO_STATUS_OK) { return -2; }
    gpio_dir_t d=GPIO_DIR_INPUT; (void)gpio_get_direction(0u,&d);
    return (d==GPIO_DIR_OUTPUT)?0:-3;
}
