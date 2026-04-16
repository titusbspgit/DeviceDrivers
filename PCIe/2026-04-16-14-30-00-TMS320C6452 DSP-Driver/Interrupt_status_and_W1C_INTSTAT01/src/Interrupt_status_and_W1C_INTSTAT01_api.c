#include "../include/Interrupt_status_and_W1C_INTSTAT01_api.h"

int Interrupt_status_and_W1C_INTSTAT01_feature_run(void)
{
    if (gpio_init(TMS320C6452_GPIO_BASE_ADDR) != GPIO_STATUS_OK) { return -1; }
    (void)gpio_irq_clear_status(0xFFFFFFFFu);
    (void)gpio_irq_bank_enable(0u, true);

    (void)gpio_set_direction(0u, GPIO_DIR_OUTPUT);
    (void)gpio_irq_configure_pin(0u, GPIO_EDGE_BOTH);

    (void)gpio_write_pin(0u,false);
    (void)gpio_write_pin(0u,true);

    uint32_t st=0U; (void)gpio_irq_get_status(&st);
    bool pending = ((st & GPIO_PIN_MASK(0u)) != 0U);

    (void)gpio_irq_clear_status(GPIO_PIN_MASK(0u));
    st=0U; (void)gpio_irq_get_status(&st);
    bool cleared = ((st & GPIO_PIN_MASK(0u)) == 0U);

    return (pending && cleared) ? 0 : -2;
}
