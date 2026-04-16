#include "../include/Peripheral_Identification_PID_api.h"

int Peripheral_Identification_PID_feature_run(uint32_t *pid_out)
{
    if (gpio_init(TMS320C6452_GPIO_BASE_ADDR) != GPIO_STATUS_OK) { return -1; }
    if (pid_out == (uint32_t*)0) { return -2; }
    if (gpio_get_pid(pid_out) != GPIO_STATUS_OK) { return -3; }
    return 0;
}
