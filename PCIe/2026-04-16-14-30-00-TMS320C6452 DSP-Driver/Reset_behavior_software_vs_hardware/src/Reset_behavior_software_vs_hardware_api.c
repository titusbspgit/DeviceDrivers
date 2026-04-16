#include "../include/Reset_behavior_software_vs_hardware_api.h"

int Reset_behavior_software_vs_hardware_feature_run(void)
{
    /* On emulator reset, GPIO may retain state; we ensure APIs continue to function */
    if (gpio_init(TMS320C6452_GPIO_BASE_ADDR) != GPIO_STATUS_OK) { return -1; }
    (void)gpio_set_direction(0u, GPIO_DIR_OUTPUT);
    (void)gpio_write_pin(0u,true);
    bool v=false; (void)gpio_read_pin(0u,&v);
    return v?0:-2;
}
