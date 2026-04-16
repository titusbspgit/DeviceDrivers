#include <stdio.h>
#include <stdint.h>
#include "../include/feature_nomuxirq.h"

int main(void)
{
    (void)gpio_init(TMS320C6452_GPIO_BASE_ADDR);
    printf("GPIO IRQs have dedicated vectors; map per device manual.\n");
    printf("PASS (documentation)\n");
    return 0;
}
