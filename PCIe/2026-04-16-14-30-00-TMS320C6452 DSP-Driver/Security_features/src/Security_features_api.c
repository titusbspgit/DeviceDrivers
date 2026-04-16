#include "../include/Security_features_api.h"

int Security_features_feature_run(void)
{
    /* If platform allows, MMIO works without extra security setup */
    if (gpio_init(TMS320C6452_GPIO_BASE_ADDR) != GPIO_STATUS_OK) { return -1; }
    uint32_t pid=0U; if (gpio_get_pid(&pid) != GPIO_STATUS_OK) { return -2; }
    (void)pid; return 0;
}
