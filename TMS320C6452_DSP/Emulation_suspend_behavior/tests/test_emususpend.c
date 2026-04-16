#include <stdio.h>
#include <stdint.h>
#include "../include/feature_emususpend.h"

int main(void)
{
    (void)gpio_init(TMS320C6452_GPIO_BASE_ADDR);
    printf("Note: GPIO continues operating during emulation halt; keep external systems safe.\n");
    printf("PASS (documentation)\n");
    return 0;
}
