#include "../include/Per-bank_interrupt_enable_BINTEN_api.h"

int Per_bank_interrupt_enable_BINTEN_feature_run(void)
{
    if (gpio_init(TMS320C6452_GPIO_BASE_ADDR) != GPIO_STATUS_OK) { return -1; }
    (void)gpio_irq_clear_status(0xFFFFFFFFu);

    /* Disable then enable bank 0 and verify interrupts only when enabled */
    (void)gpio_irq_bank_enable(0u, false);
    (void)gpio_irq_configure_pin(0u, GPIO_EDGE_RISING);
    (void)gpio_set_direction(0u, GPIO_DIR_OUTPUT);
    (void)gpio_write_pin(0u,false);
    (void)gpio_write_pin(0u,true);

    uint32_t pending=0U; (void)gpio_irq_get_status(&pending);
    bool off = ((pending & GPIO_PIN_MASK(0u)) == 0U);

    (void)gpio_irq_bank_enable(0u, true);
    (void)gpio_write_pin(0u,false);
    (void)gpio_write_pin(0u,true);

    (void)gpio_irq_get_status(&pending);
    bool on = ((pending & GPIO_PIN_MASK(0u)) != 0U);
    (void)gpio_irq_clear_status(GPIO_PIN_MASK(0u));

    return (off && on) ? 0 : -2;
}
