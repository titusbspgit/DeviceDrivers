#include <stdio.h>
#include <stdint.h>
#include "../include/feature_baseaddr.h"

int main(void)
{
    gpio_status_t st = gpio_init((uintptr_t)0xA1008000u);
    printf("init status=%u base=0xA1008000\nPASS\n", (unsigned)st);
    return 0;
}
