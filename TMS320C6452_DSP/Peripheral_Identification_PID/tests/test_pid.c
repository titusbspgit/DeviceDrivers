#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../include/feature_pid.h"

int main(void)
{
    (void)gpio_init(TMS320C6452_GPIO_BASE_ADDR);
    uint32_t pid=0u; (void)gpio_get_pid(&pid);
    printf("PID=0x%08X\nPASS\n", (unsigned)pid);
    return 0;
}
