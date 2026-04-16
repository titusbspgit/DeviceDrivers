#include "../include/Input_sampling_IN_DATA01_api.h"

int Input_sampling_IN_DATA01_feature_run(void)
{
    if (gpio_init(TMS320C6452_GPIO_BASE_ADDR) != GPIO_STATUS_OK) { return -1; }

    (void)gpio_set_direction(0u, GPIO_DIR_OUTPUT);
    (void)gpio_write_pin(0u, false);
    bool v=false; (void)gpio_read_pin(0u,&v); if (v){return -2;}

    (void)gpio_write_pin(0u, true);
    (void)gpio_read_pin(0u,&v); if (!v){return -3;}

    /* Configure pin16 as input; read level (unknown externally). We only validate API returns OK. */
    (void)gpio_set_direction(16u, GPIO_DIR_INPUT);
    if (gpio_read_pin(16u,&v) != GPIO_STATUS_OK) { return -4; }
    return 0;
}
