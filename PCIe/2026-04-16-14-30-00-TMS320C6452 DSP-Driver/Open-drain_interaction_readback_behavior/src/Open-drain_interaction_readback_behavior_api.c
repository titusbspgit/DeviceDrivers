#include "../include/Open-drain_interaction_readback_behavior_api.h"

int Open_drain_interaction_readback_behavior_feature_run(void)
{
    /* On standard push-pull pins, readback tracks driven value */
    if (gpio_init(TMS320C6452_GPIO_BASE_ADDR) != GPIO_STATUS_OK) { return -1; }
    (void)gpio_set_direction(0u, GPIO_DIR_OUTPUT);

    (void)gpio_write_pin(0u,false); bool v=false; (void)gpio_read_pin(0u,&v); if (v){return -2;}
    (void)gpio_write_pin(0u,true);  (void)gpio_read_pin(0u,&v); if (!v){return -3;}
    return 0;
}
