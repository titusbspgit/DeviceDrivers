#include "../include/Controller_base_address_api.h"

int Controller_base_address_feature_run(void)
{
    if (gpio_init((uintptr_t)0xA1008000u) != GPIO_STATUS_OK) { return -1; }
    uint32_t pid=0U; if (gpio_get_pid(&pid) != GPIO_STATUS_OK) { return -2; }
    (void)pid; return 0;
}
