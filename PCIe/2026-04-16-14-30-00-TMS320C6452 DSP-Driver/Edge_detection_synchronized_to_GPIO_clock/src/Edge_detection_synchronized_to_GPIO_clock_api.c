#include "../include/Edge_detection_synchronized_to_GPIO_clock_api.h"

static void delay(volatile uint32_t n){ while(n--!=0u){ __asm__ __volatile__("":::"memory"); } }

int Edge_detection_synchronized_to_GPIO_clock_feature_run(void)
{
    if (gpio_init(TMS320C6452_GPIO_BASE_ADDR) != GPIO_STATUS_OK) { return -1; }
    (void)gpio_irq_clear_status(0xFFFFFFFFu);
    (void)gpio_irq_bank_enable(0u, true);
    (void)gpio_set_direction(0u, GPIO_DIR_OUTPUT);
    (void)gpio_irq_configure_pin(0u, GPIO_EDGE_RISING);
    (void)gpio_write_pin(0u,false); delay(1000u);
    (void)gpio_write_pin(0u,true);  delay(1000u);
    uint32_t st=0U; (void)gpio_irq_get_status(&st);
    bool got = ((st & GPIO_PIN_MASK(0u)) != 0U);
    (void)gpio_irq_clear_status(GPIO_PIN_MASK(0u));
    return got?0:-2;
}
