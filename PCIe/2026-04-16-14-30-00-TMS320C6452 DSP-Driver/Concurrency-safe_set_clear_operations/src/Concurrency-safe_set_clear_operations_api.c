#include "../include/Concurrency-safe_set_clear_operations_api.h"

int Concurrency_safe_set_clear_operations_feature_run(void)
{
    if (gpio_init(TMS320C6452_GPIO_BASE_ADDR) != GPIO_STATUS_OK) { return -1; }
    (void)gpio_set_direction(0u, GPIO_DIR_OUTPUT);
    (void)gpio_set_direction(1u, GPIO_DIR_OUTPUT);

    (void)gpio_write_mask(GPIO_PIN_MASK(0u), GPIO_PIN_MASK(1u));
    bool v0=false, v1=true; (void)gpio_read_pin(0u,&v0); (void)gpio_read_pin(1u,&v1);
    return (v0 && !v1) ? 0 : -2;
}
