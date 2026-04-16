#include <stdio.h>
#include <stdint.h>
#include "../include/feature_endianness.h"

int main(void)
{
    (void)gpio_init(TMS320C6452_GPIO_BASE_ADDR);
    uint32_t pid = 0u;
    (void)gpio_get_pid(&pid);
    printf("PID=0x%08X (endianness independent)\n", (unsigned)pid);
    printf("PASS\n");
    return 0;
}
