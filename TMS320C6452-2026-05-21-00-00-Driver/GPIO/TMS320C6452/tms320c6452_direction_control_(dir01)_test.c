#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "tms320c6452_direction_control_(dir01).h"
#include "tms320c6452_gpio_core.h"

static void show(const char *tag)
{
    printf("%s DIR=0x%08X\n", tag, tms320c6452_direction_get_mask());
}

int main(void)
{
    int ret = tms320c6452_direction_control_init();
    if (ret) {
        printf("init failed %d\n", ret);
        return ret;
    }
    show("after_init");

    /* Set pins 0..3 as outputs */
    uint32_t dir = 0xFFFFFFF0u; /* 1=input, 0=output for 0..3 */
    tms320c6452_direction_set_mask(dir);
    show("after_set_mask");

    /* Toggle a single pin direction */
    tms320c6452_direction_set_pin(1u, 1); /* set as input */
    tms320c6452_direction_set_pin(2u, 0); /* set as output */
    show("after_single_pin_updates");

    /* Edge case: invalid pin */
    ret = tms320c6452_direction_set_pin(33u, 1);
    if (ret != 0) {
        printf("invalid pin rejected as expected (ret=%d)\n", ret);
    }

    return 0;
}
