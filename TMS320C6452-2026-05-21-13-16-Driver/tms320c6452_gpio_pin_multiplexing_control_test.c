#include <stdint.h>
#include <stdio.h>
#include "tms320c6452_gpio_pin_multiplexing_control.h"

static int test_basic_sequence(void)
{
    tms320c6452_gpio_pinmux_ctx_t ctx;
    int rc = 0;

    if (tms320c6452_gpio_pin_multiplexing_control_init(&ctx, 0u /* UNKNOWN base */) != TMS320C6452_GPIO_EOK) rc++;
    if (tms320c6452_gpio_pin_multiplexing_control_unlock(&ctx) != TMS320C6452_GPIO_EUNSUPPORTED) rc++;
    if (tms320c6452_gpio_pin_multiplexing_control_set(&ctx, 0u, 0u) != TMS320C6452_GPIO_EUNSUPPORTED) rc++;
    uint32_t func = 0xFFFFFFFFu;
    if (tms320c6452_gpio_pin_multiplexing_control_get(&ctx, 0u, &func) != TMS320C6452_GPIO_EUNSUPPORTED) rc++;
    if (tms320c6452_gpio_pin_multiplexing_control_lock(&ctx) != TMS320C6452_GPIO_EUNSUPPORTED) rc++;

    return rc;
}

static int test_edge_cases(void)
{
    tms320c6452_gpio_pinmux_ctx_t ctx;
    int rc = 0;

    if (tms320c6452_gpio_pin_multiplexing_control_init(&ctx, 0u) != TMS320C6452_GPIO_EOK) rc++;
    if (tms320c6452_gpio_pin_multiplexing_control_get(&ctx, 1u, (uint32_t*)0) != TMS320C6452_GPIO_EINVAL) rc++;

    return rc;
}

int main(void)
{
    int rc = 0;
    rc += test_basic_sequence();
    rc += test_edge_cases();

    if (rc == 0) {
        printf("PASS\n");
    } else {
        printf("FAIL:%d\n", rc);
    }
    return (rc == 0) ? 0 : 1;
}

/* GIT_STATUS
Repository: https://github.com/titusbspgit/DeviceDrivers
Branch: main
Last commit contains this push. */
