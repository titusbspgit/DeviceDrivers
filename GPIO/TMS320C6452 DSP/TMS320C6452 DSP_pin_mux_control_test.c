#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "TMS320C6452 DSP_pin_mux_control.h"

static int test_init_and_table_mode(void)
{
    int32_t rc;

    /* Allow NULL ops to use table mode safely */
    rc = gpio_pinmux_init((const struct gpio_core_ops*)0);
    if (rc != GPIO_EOK) {
        return 1;
    }
    return 0;
}

static int test_set_get_valid(void)
{
    int32_t rc;
    uint32_t out = 0u;

    rc = gpio_pinmux_config(5u, 2u);
    if (rc != GPIO_EOK) {
        return 1;
    }
    rc = gpio_pinmux_get(5u, &out);
    if (rc != GPIO_EOK) {
        return 1;
    }
    if (out != 2u) {
        return 1;
    }

    rc = gpio_pinmux_config(0u, 0u);
    if (rc != GPIO_EOK) {
        return 1;
    }
    rc = gpio_pinmux_get(0u, &out);
    if (rc != GPIO_EOK || out != 0u) {
        return 1;
    }

    rc = gpio_pinmux_config(31u, 3u);
    if (rc != GPIO_EOK) {
        return 1;
    }
    rc = gpio_pinmux_get(31u, &out);
    if (rc != GPIO_EOK || out != 3u) {
        return 1;
    }

    return 0;
}

static int test_edge_cases(void)
{
    int32_t rc;
    uint32_t out = 0u;

    rc = gpio_pinmux_config(32u, 1u); /* invalid pin */
    if (rc == GPIO_EOK) {
        return 1;
    }

    rc = gpio_pinmux_get(32u, &out); /* invalid pin */
    if (rc == GPIO_EOK) {
        return 1;
    }

    rc = gpio_pinmux_get(1u, (uint32_t*)0); /* null out ptr */
    if (rc == GPIO_EOK) {
        return 1;
    }

    return 0;
}

int main(void)
{
    if (test_init_and_table_mode() != 0) {
        printf("FAIL:init\n");
        return 1;
    }
    if (test_set_get_valid() != 0) {
        printf("FAIL:set_get\n");
        return 1;
    }
    if (test_edge_cases() != 0) {
        printf("FAIL:edges\n");
        return 1;
    }

    printf("OK\n");
    return 0;
}
