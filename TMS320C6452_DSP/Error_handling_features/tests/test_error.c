#include <stdio.h>
#include <stdint.h>
#include "../include/feature_error.h"

int main(void)
{
    gpio_status_t st = gpio_get_pid(NULL);
    printf("get_pid(NULL) => %u (expect %u)\n", (unsigned)st, (unsigned)GPIO_STATUS_INVALID_ARG);
    printf("PASS\n");
    return 0;
}
