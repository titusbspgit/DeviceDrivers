#include "../include/Output_drive_control_OUT_SET_CLR_api.h"

static void delay(volatile uint32_t n){ while(n--!=0u){ __asm__ __volatile__("":::"memory"); } }

int Output_drive_control_OUT_SET_CLR_feature_run(void)
{
    if (gpio_init(TMS320C6452_GPIO_BASE_ADDR) != GPIO_STATUS_OK) { return -1; }
    (void)gpio_set_direction(0u, GPIO_DIR_OUTPUT);
    (void)gpio_write_pin(0u, false);
    delay(1000u);
    bool v=false; (void)gpio_read_pin(0u,&v); if (v){return -2;}
    (void)gpio_write_pin(0u, true);
    delay(1000u);
    (void)gpio_read_pin(0u,&v); if (!v){return -3;}

    /* Bulk mask: set pin0, clear pin16 */
    (void)gpio_set_direction(16u, GPIO_DIR_OUTPUT);
    (void)gpio_write_mask(GPIO_PIN_MASK(0u), GPIO_PIN_MASK(16u));
    bool v0=false,v16=true; (void)gpio_read_pin(0u,&v0); (void)gpio_read_pin(16u,&v16);
    return (v0 && !v16)?0:-4;
}
