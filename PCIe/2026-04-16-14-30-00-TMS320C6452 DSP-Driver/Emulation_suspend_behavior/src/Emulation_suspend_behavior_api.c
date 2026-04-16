#include "../include/Emulation_suspend_behavior_api.h"

int Emulation_suspend_behavior_feature_run(void)
{
    if (gpio_init(TMS320C6452_GPIO_BASE_ADDR) != GPIO_STATUS_OK) { return -1; }
    (void)gpio_set_direction(0u, GPIO_DIR_OUTPUT);
    (void)gpio_write_pin(0u,true);
    bool v=false; (void)gpio_read_pin(0u,&v);
    return v?0:-2;
}
