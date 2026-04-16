#include "../include/Error_handling_features_api.h"

int Error_handling_features_feature_run(void)
{
    /* Access before init */
    bool val=false; gpio_status_t s;
    s = gpio_read_pin(0u, &val); if (s != GPIO_STATUS_NOT_INITIALIZED) { return -1; }

    (void)gpio_init(TMS320C6452_GPIO_BASE_ADDR);

    /* Invalid pointer */
    s = gpio_get_pid((uint32_t*)0); if (s != GPIO_STATUS_INVALID_ARG) { return -2; }

    /* Invalid pin */
    s = gpio_set_direction(32u, GPIO_DIR_OUTPUT); if (s != GPIO_STATUS_INVALID_ARG) { return -3; }

    /* Invalid bank */
    uint32_t mask=0U; s = gpio_isr_service(5u, &mask); if (s != GPIO_STATUS_INVALID_ARG) { return -4; }

    return 0;
}
