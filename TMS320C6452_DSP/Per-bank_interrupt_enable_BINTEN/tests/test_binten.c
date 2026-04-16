#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../include/feature_binten.h"

int main(void)
{
    (void)gpio_init(TMS320C6452_GPIO_BASE_ADDR);

    (void)gpio_irq_bank_enable(0u, false);
    (void)gpio_irq_bank_enable(1u, false);

    uint32_t pid=0u; (void)gpio_get_pid(&pid);
    printf("PID=0x%08X\n", (unsigned)pid);

    /* Re-enable bank0 and bank1 */
    (void)gpio_irq_bank_enable(0u, true);
    (void)gpio_irq_bank_enable(1u, true);

    printf("PASS\n");
    return 0;
}
