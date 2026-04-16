#include "../include/Low-power_behavior_via_PSC_api.h"

int Low_power_behavior_via_PSC_feature_run(void)
{
    /* Simulate exit from low-power by calling init again and verifying IO */
    if (gpio_init(TMS320C6452_GPIO_BASE_ADDR) != GPIO_STATUS_OK) { return -1; }
    (void)gpio_set_direction(0u, GPIO_DIR_OUTPUT);
    (void)gpio_write_pin(0u,true);
    bool v=false; (void)gpio_read_pin(0u,&v);
    return v?0:-2;
}
