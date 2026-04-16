#include "../include/GPIO_banked_architecture_32_signals_2_banks_api.h"

int GPIO_banked_architecture_32_signals_2_banks_feature_run(void)
{
    if (gpio_init(TMS320C6452_GPIO_BASE_ADDR) != GPIO_STATUS_OK) { return -1; }

    /* Configure pin0 (bank0) and pin16 (bank1) as outputs */
    (void)gpio_set_direction(0u, GPIO_DIR_OUTPUT);
    (void)gpio_set_direction(16u, GPIO_DIR_OUTPUT);

    /* Drive distinct values per bank and verify independent readback */
    (void)gpio_write_pin(0u, true);
    (void)gpio_write_pin(16u, false);

    bool v0=false, v16=true; (void)gpio_read_pin(0u,&v0); (void)gpio_read_pin(16u,&v16);
    if (!(v0 && !v16)) { return -2; }

    (void)gpio_write_pin(0u, false);
    (void)gpio_write_pin(16u, true);
    (void)gpio_read_pin(0u,&v0); (void)gpio_read_pin(16u,&v16);
    if (!(!v0 && v16)) { return -3; }

    /* Invalid bank check via ISR service */
    uint32_t cleared=0U; gpio_status_t s;
    s = gpio_isr_service(2u, &cleared); /* invalid bank (only 0..1 valid) */
    (void)cleared;
    return (s == GPIO_STATUS_INVALID_ARG) ? 0 : -4;
}
