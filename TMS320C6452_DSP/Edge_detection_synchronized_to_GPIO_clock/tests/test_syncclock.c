#include <stdio.h>
#include <stdint.h>
#include "../include/feature_syncclock.h"

int main(void)
{
    (void)gpio_init(TMS320C6452_GPIO_BASE_ADDR);
    printf("Note: Edge detection latency is bounded by GPIO clock (CPU/6).\n");
    printf("PASS (documentation)\n");
    return 0;
}
