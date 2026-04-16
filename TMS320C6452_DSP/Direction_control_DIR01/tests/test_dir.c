#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../include/feature_dir.h"

int main(void)
{
    (void)gpio_init(TMS320C6452_GPIO_BASE_ADDR);
    gpio_status_t st;
    st = gpio_set_direction(0u, GPIO_DIR_INPUT);
    printf("set dir pin0=input: %u\n", (unsigned)st);
    gpio_dir_t d = GPIO_DIR_OUTPUT;
    (void)gpio_get_direction(0u, &d);
    printf("read dir pin0: %u (1=input)\n", (unsigned)d);
    printf("%s\n", (d == GPIO_DIR_INPUT) ? "PASS" : "FAIL");
    return (d == GPIO_DIR_INPUT) ? 0 : 1;
}
