#include "../include/Endianness_independence_api.h"

int Endianness_independence_feature_run(void)
{
    /* Behavior should be identical; this test just performs basic IO in big-endian build */
    if (gpio_init(TMS320C6452_GPIO_BASE_ADDR) != GPIO_STATUS_OK) { return -1; }
    (void)gpio_set_direction(0u, GPIO_DIR_OUTPUT);
    (void)gpio_write_pin(0u, true);
    bool v=false; (void)gpio_read_pin(0u,&v);
    return v?0:-2;
}
