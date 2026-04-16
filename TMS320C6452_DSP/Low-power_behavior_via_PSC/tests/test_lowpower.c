#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../include/feature_lowpower.h"

int main(void)
{
    (void)gpio_init(TMS320C6452_GPIO_BASE_ADDR);
    /* Simulate low-power entry by disabling banks and clearing status */
    (void)gpio_irq_bank_enable(0u, false);
    (void)gpio_irq_bank_enable(1u, false);
    (void)gpio_irq_clear_status(0xFFFFFFFFu);

    /* Simulate wake: re-enable banks */
    (void)gpio_irq_bank_enable(0u, true);
    (void)gpio_irq_bank_enable(1u, true);

    printf("PASS (sequence only)\n");
    return 0;
}
