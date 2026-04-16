#include "../include/Direction_control_DIR01_api.h"

int Direction_control_DIR01_feature_run(void)
{
    if (gpio_init(TMS320C6452_GPIO_BASE_ADDR) != GPIO_STATUS_OK) { return -1; }
    if (gpio_set_direction(0u, GPIO_DIR_OUTPUT) != GPIO_STATUS_OK) { return -2; }
    if (gpio_set_direction(16u, GPIO_DIR_INPUT) != GPIO_STATUS_OK) { return -3; }

    gpio_dir_t d0 = GPIO_DIR_INPUT, d1 = GPIO_DIR_OUTPUT;
    if (gpio_get_direction(0u, &d0) != GPIO_STATUS_OK) { return -4; }
    if (gpio_get_direction(16u, &d1) != GPIO_STATUS_OK) { return -5; }

    return ((d0 == GPIO_DIR_OUTPUT) && (d1 == GPIO_DIR_INPUT)) ? 0 : -6;
}
