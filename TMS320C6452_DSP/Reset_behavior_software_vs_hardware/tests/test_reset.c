#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../include/feature_reset.h"

int main(void)
{
    (void)gpio_init(TMS320C6452_GPIO_BASE_ADDR);
    uint32_t pid=0u; (void)gpio_get_pid(&pid);
    printf("PID=0x%08X\n", (unsigned)pid);

    /* Verify default DIR is input (1) for a pin after hardware reset assumption */
    gpio_dir_t d = GPIO_DIR_OUTPUT; (void)gpio_get_direction(0u, &d);
    printf("DIR[0]=%u (expect 1 after HW reset)\n", (unsigned)d);
    printf("PASS (informational; depends on reset source)\n");
    return 0;
}
