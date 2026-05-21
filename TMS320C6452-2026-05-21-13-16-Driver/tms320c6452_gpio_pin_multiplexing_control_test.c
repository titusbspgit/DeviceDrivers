#include <stdint.h>
#include "tms320c6452_gpio_pin_multiplexing_control_drv.h"

static void test_basic_sequence(void)
{
    tms320c6452_gpio_pinmux_ctx_t ctx;
    (void)tms320c6452_gpio_pinmux_init(&ctx, 0u /* UNKNOWN base */);
    (void)tms320c6452_gpio_pinmux_unlock(&ctx);
    (void)tms320c6452_gpio_pinmux_set(&ctx, 0u, 0u);
    uint32_t func = 0xFFFFFFFFu;
    (void)tms320c6452_gpio_pinmux_get(&ctx, 0u, &func);
    (void)tms320c6452_gpio_pinmux_lock(&ctx);
}

static void test_edge_cases(void)
{
    tms320c6452_gpio_pinmux_ctx_t ctx;
    (void)tms320c6452_gpio_pinmux_init(&ctx, 0u);
    (void)tms320c6452_gpio_pinmux_set(&ctx, 0xFFFFFFFFu, 3u);
    (void)tms320c6452_gpio_pinmux_get(&ctx, 0xFFFFFFFFu, (uint32_t*)0);
}

int main(void)
{
    test_basic_sequence();
    test_edge_cases();
    return 0;
}
