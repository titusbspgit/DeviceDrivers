#include <stdint.h>
#include <stdbool.h>
#include "tms320c6452_gpio_pin_multiplexing_control.h"

/* Simple test harness without stdio: returns 0 on success, nonzero on failure */

/* Emulated device pinmux registers for testing */
static volatile uint32_t PINMUX_REG_A = 0u; /* controls GPIO[0..7], 2-bit fields each */
static volatile uint32_t PINMUX_REG_B = 0u; /* controls GPIO[8..15], 2-bit fields each */

static tms320c6452_gpio_pinmux_field_t g_pin_fields[32];

static void build_test_mapping(void)
{
    /* Define 2-bit fields for GPIO[0..7] on REG_A, LSB = pin*2 */
    for (uint32_t i = 0; i < 8u; ++i) {
        g_pin_fields[i].reg = &PINMUX_REG_A;
        g_pin_fields[i].lsb = (uint8_t)(i * 2u);
        g_pin_fields[i].width = 2u;
    }
    /* Define 2-bit fields for GPIO[8..15] on REG_B */
    for (uint32_t i = 8u; i < 16u; ++i) {
        g_pin_fields[i].reg = &PINMUX_REG_B;
        g_pin_fields[i].lsb = (uint8_t)((i - 8u) * 2u);
        g_pin_fields[i].width = 2u;
    }
    /* The rest are UNKNOWN/unsupported in this test */
    for (uint32_t i = 16u; i < 32u; ++i) {
        g_pin_fields[i].reg = NULL; /* UNKNOWN */
        g_pin_fields[i].lsb = 0u;
        g_pin_fields[i].width = 0u; /* UNKNOWN */
    }
}

static int run_tests(void)
{
    int rc;
    uint32_t out;
    int failures = 0;

    build_test_mapping();

    tms320C6452_gpio_pinmux_cfg_t cfg = {
        .map = g_pin_fields,
        .count = 32u
    };

    rc = tms320c6452_gpio_pinmux_init(&cfg);
    if (rc != TMS320C6452_GPIO_PINMUX_OK) { ++failures; }
    if (!tms320c6452_gpio_pinmux_is_initialized()) { ++failures; }

    /* Configure GPIO3 function to 2 (binary 10), affects bits [7:6] of REG_A */
    rc = tms320c6452_gpio_pinmux_set(3u, 2u);
    if (rc != TMS320C6452_GPIO_PINMUX_OK) { ++failures; }
    if (PINMUX_REG_A != (2u << (3u * 2u))) { ++failures; }

    /* Read back */
    out = 0xFFFFFFFFu;
    rc = tms320c6452_gpio_pinmux_get(3u, &out);
    if (rc != TMS320C6452_GPIO_PINMUX_OK) { ++failures; }
    if (out != 2u) { ++failures; }

    /* Change GPIO0 to 1, verify non-overlap with GPIO3 */
    rc = tms320c6452_gpio_pinmux_set(0u, 1u);
    if (rc != TMS320C6452_GPIO_PINMUX_OK) { ++failures; }
    if (PINMUX_REG_A != ((2u << 6u) | (1u << 0u))) { ++failures; }

    /* Invalid: function code out of range for 2-bit field (e.g., 4) */
    rc = tms320c6452_gpio_pinmux_set(1u, 4u);
    if (rc != TMS320C6452_GPIO_PINMUX_EINVAL) { ++failures; }

    /* Invalid: unknown mapping (GPIO20) */
    rc = tms320c6452_gpio_pinmux_set(20u, 1u);
    if (rc != TMS320C6452_GPIO_PINMUX_ESTATE) { ++failures; }

    /* Invalid: gpio index out of range */
    rc = tms320c6452_gpio_pinmux_set(33u, 0u);
    if (rc != TMS320C6452_GPIO_PINMUX_EINVAL) { ++failures; }

    /* Get on unknown mapping should fail */
    rc = tms320c6452_gpio_pinmux_get(25u, &out);
    if (rc != TMS320C6452_GPIO_PINMUX_ESTATE) { ++failures; }

    /* Get with NULL out pointer should fail */
    rc = tms320c6452_gpio_pinmux_get(0u, NULL);
    if (rc != TMS320C6452_GPIO_PINMUX_EINVAL) { ++failures; }

    return failures;
}

int main(void)
{
    int failures = run_tests();
    return (failures == 0) ? 0 : 1;
}
