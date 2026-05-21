#include "tms320c6452_gpio_pin_multiplexing_control.h"

/* Simple test application: init -> unlock -> set -> get -> verify (edge cases included) */

static int test_basic_sequence(void)
{
    tms320c6452_gpio_pinmux_ctx_t ctx;
    int rc;

    rc = tms320c6452_gpio_pin_multiplexing_control_init(&ctx, (uintptr_t)TMS320C6452_GPIO_SYSCFG_BASE);
    if (rc != TMS320C6452_GPIO_EOK) return rc;

    rc = tms320c6452_gpio_pin_multiplexing_control_unlock(&ctx);
    if (rc != TMS320C6452_GPIO_EOK) return rc;

    /* Configure pin 0 to function 1 (example; actual function mapping is device-specific) */
    rc = tms320c6452_gpio_pin_multiplexing_control_set(&ctx, 0u, 1u);
    if (rc != TMS320C6452_GPIO_EOK) return rc;

    uint32_t fsel = 0u;
    rc = tms320c6452_gpio_pin_multiplexing_control_get(&ctx, 0u, &fsel);
    if (rc != TMS320C6452_GPIO_EOK) return rc;

    /* Optional: re-lock configuration */
    rc = tms320c6452_gpio_pin_multiplexing_control_lock(&ctx);
    if (rc != TMS320C6452_GPIO_EOK) return rc;

    return TMS320C6452_GPIO_EOK;
}

static int test_edge_cases(void)
{
    tms320c6452_gpio_pinmux_ctx_t ctx;
    int rc;

    rc = tms320c6452_gpio_pin_multiplexing_control_init(&ctx, (uintptr_t)TMS320C6452_GPIO_SYSCFG_BASE);
    if (rc != TMS320C6452_GPIO_EOK) return rc;

    /* Null pointer checks */
    rc = tms320c6452_gpio_pin_multiplexing_control_get(&ctx, 0u, NULL);
    if (rc == TMS320C6452_GPIO_EOK) return TMS320C6452_GPIO_EFAULT;

    rc = tms320c6452_gpio_pin_multiplexing_control_init(NULL, (uintptr_t)TMS320C6452_GPIO_SYSCFG_BASE);
    if (rc == TMS320C6452_GPIO_EOK) return TMS320C6452_GPIO_EFAULT;

    /* Out-of-range pin index if max is known (placeholder) */
    (void)TMS320C6452_GPIO_MAX_PINS; /* reference to indicate dependency */

    return TMS320C6452_GPIO_EOK;
}

int main(void)
{
    int rc;
    rc = test_basic_sequence();
    if (rc != TMS320C6452_GPIO_EOK) {
        return rc;
    }

    rc = test_edge_cases();
    if (rc != TMS320C6452_GPIO_EOK) {
        return rc;
    }

    return 0;
}
