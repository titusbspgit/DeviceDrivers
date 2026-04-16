#include "../include/Initialization_sequence_api.h"

/* Provide local HAL overrides for test context */
bool gpio_hal_enable_clock(void) __attribute__((weak));
bool gpio_hal_enable_clock(void) { return true; }

bool gpio_hal_configure_pinmux(uint32_t pin) __attribute__((weak));
bool gpio_hal_configure_pinmux(uint32_t pin) { (void)pin; return true; }

int Initialization_sequence_feature_run(void)
{
    if (gpio_init(TMS320C6452_GPIO_BASE_ADDR) != GPIO_STATUS_OK) { return -1; }

    /* Basic sanity: read PID, clear interrupts, set default directions */
    uint32_t pid = 0U;
    if (gpio_get_pid(&pid) != GPIO_STATUS_OK) { return -2; }

    (void)gpio_irq_clear_status(0xFFFFFFFFu);

    /* Configure pin 0 as output, pin 16 as input */
    if (gpio_set_direction(0u, GPIO_DIR_OUTPUT) != GPIO_STATUS_OK) { return -3; }
    if (gpio_set_direction(16u, GPIO_DIR_INPUT)  != GPIO_STATUS_OK) { return -4; }

    /* Write then readback */
    (void)gpio_write_pin(0u, true);
    bool v=false;
    if (gpio_read_pin(0u, &v) != GPIO_STATUS_OK) { return -5; }

    (void)pid; /* silence unused in some builds */
    return (v ? 0 : -6);
}
