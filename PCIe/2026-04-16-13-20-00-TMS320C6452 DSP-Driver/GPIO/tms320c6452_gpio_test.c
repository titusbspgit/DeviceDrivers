/*
 * TMS320C6452 GPIO Driver - Test Application (Baremetal)
 * RCI-ag-DrvGen Agent
 *
 * This self-test exercises basic GPIO functionality per SPRUF95 without relying on CPU interrupt controller.
 * It uses banked interrupt status (INTSTAT01) and SET/CLR registers to generate/observe edges.
 */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "tms320c6452_gpio.h"

#ifndef TEST_GPIO_BASE
#define TEST_GPIO_BASE   (TMS320C6452_GPIO_BASE_DEFAULT)
#endif

static unsigned g_pass = 0u;
static unsigned g_fail = 0u;

static void report(const char * name, bool ok)
{
    if (ok)
    {
        ++g_pass;
        printf("[PASS] %s\n", name);
    }
    else
    {
        ++g_fail;
        printf("[FAIL] %s\n", name);
    }
}

static bool expect_equal_u32(const char * what, uint32_t a, uint32_t b)
{
    const bool ok = (a == b);
    if (!ok)
    {
        printf("    Mismatch for %s: 0x%08lX != 0x%08lX\n", what, (unsigned long)a, (unsigned long)b);
    }
    return ok;
}

static bool expect_true(const char * what, bool v)
{
    if (!v)
    {
        printf("    Expected true: %s\n", what);
    }
    return v;
}

static bool expect_false(const char * what, bool v)
{
    if (v)
    {
        printf("    Expected false: %s\n", what);
    }
    return !v;
}

int main(void)
{
    tms320c6452_gpio_t gpio;
    bool ok;

    /* Init */
    tms320c6452_gpio_status_t st = tms320c6452_gpio_init(&gpio, (uintptr_t)TEST_GPIO_BASE, true);
    report("init (PID verify)", st == TMS320C6452_GPIO_STATUS_OK);

    /* PID validation */
    const uint32_t pid = tms320c6452_gpio_get_pid(&gpio);
    const uint32_t pid_func = (pid & TMS320C6452_GPIO_PID_FUNCTION_MASK) >> TMS320C6452_GPIO_PID_FUNCTION_SHIFT;
    const uint32_t pid_major = (pid & TMS320C6452_GPIO_PID_MAJOR_MASK) >> TMS320C6452_GPIO_PID_MAJOR_SHIFT;
    const uint32_t pid_minor = (pid & TMS320C6452_GPIO_PID_MINOR_MASK) >> TMS320C6452_GPIO_PID_MINOR_SHIFT;
    ok = expect_equal_u32("PID.FUNCTION", pid_func, TMS320C6452_GPIO_PID_FUNCTION_GPIO);
    report("pid function matches", ok);
    /* Known from SPRUF95: MAJOR=1, MINOR=5 (may vary by silicon; treat as soft check) */
    report("pid major nonzero", (pid_major != 0u));
    report("pid minor nonzero", (pid_minor != 0u));

    /* Choose pins for tests */
    const uint32_t pin_out0 = 7u;   /* bank0 */
    const uint32_t pin_out1 = 20u;  /* bank1 */

    /* Ensure clean state: set selected pins to outputs and low */
    st = tms320c6452_gpio_set_outputs_mask(&gpio, TMS320C6452_GPIO_PIN_MASK(pin_out0) | TMS320C6452_GPIO_PIN_MASK(pin_out1));
    report("set outputs mask", st == TMS320C6452_GPIO_STATUS_OK);
    st = tms320c6452_gpio_clear_pins(&gpio, TMS320C6452_GPIO_PIN_MASK(pin_out0) | TMS320C6452_GPIO_PIN_MASK(pin_out1));
    report("clear outputs (low)", st == TMS320C6452_GPIO_STATUS_OK);

    /* Verify OUT and IN for low */
    bool lvl = true;
    st = tms320c6452_gpio_read_pin(&gpio, pin_out0, &lvl);
    ok = (st == TMS320C6452_GPIO_STATUS_OK) && expect_false("pin_out0 low after clear", lvl);
    report("read pin low (bank0)", ok);

    /* Drive high and verify */
    st = tms320c6452_gpio_write_pin(&gpio, pin_out0, true);
    report("write pin high (bank0)", st == TMS320C6452_GPIO_STATUS_OK);
    st = tms320c6452_gpio_read_pin(&gpio, pin_out0, &lvl);
    ok = (st == TMS320C6452_GPIO_STATUS_OK) && expect_true("pin_out0 high after set", lvl);
    report("read pin high (bank0)", ok);

    /* Toggle back to low */
    st = tms320c6452_gpio_toggle_pin(&gpio, pin_out0);
    report("toggle pin (bank0)", st == TMS320C6452_GPIO_STATUS_OK);
    st = tms320c6452_gpio_read_pin(&gpio, pin_out0, &lvl);
    ok = (st == TMS320C6452_GPIO_STATUS_OK) && expect_false("pin_out0 low after toggle", lvl);
    report("read pin low after toggle (bank0)", ok);

    /* Bank enable checks */
    st = tms320c6452_gpio_bank_enable(&gpio, TMS320C6452_GPIO_BANK0, true);
    report("enable bank0 event", st == TMS320C6452_GPIO_STATUS_OK);
    st = tms320c6452_gpio_bank_enable(&gpio, TMS320C6452_GPIO_BANK1, true);
    report("enable bank1 event", st == TMS320C6452_GPIO_STATUS_OK);

    /* Interrupt edge detection on bank0 pin: rising */
    st = tms320c6452_gpio_config_edge(&gpio, pin_out0, true, false);
    report("config edge rising only (bank0)", st == TMS320C6452_GPIO_STATUS_OK);
    /* Clear any stale pending */
    tms320c6452_gpio_clear_pending(&gpio, 0xFFFFFFFFu);
    /* Generate rising edge */
    (void)tms320c6452_gpio_write_pin(&gpio, pin_out0, false);
    (void)tms320c6452_gpio_write_pin(&gpio, pin_out0, true);
    uint32_t pending = tms320c6452_gpio_get_pending(&gpio);
    ok = ((pending & TMS320C6452_GPIO_PIN_MASK(pin_out0)) != 0u);
    report("INTSTAT has rising (bank0)", ok);
    tms320c6452_gpio_clear_pending(&gpio, TMS320C6452_GPIO_PIN_MASK(pin_out0));

    /* Configure both-edge on bank1 pin and test rising and falling */
    st = tms320c6452_gpio_config_edge(&gpio, pin_out1, true, true);
    report("config edge both (bank1)", st == TMS320C6452_GPIO_STATUS_OK);
    tms320c6452_gpio_clear_pending(&gpio, 0xFFFFFFFFu);
    (void)tms320c6452_gpio_write_pin(&gpio, pin_out1, false);
    (void)tms320c6452_gpio_write_pin(&gpio, pin_out1, true);
    pending = tms320c6452_gpio_get_pending(&gpio);
    ok = ((pending & TMS320C6452_GPIO_PIN_MASK(pin_out1)) != 0u);
    report("INTSTAT has rising (bank1)", ok);
    tms320c6452_gpio_clear_pending(&gpio, TMS320C6452_GPIO_PIN_MASK(pin_out1));
    (void)tms320c6452_gpio_write_pin(&gpio, pin_out1, false);
    pending = tms320c6452_gpio_get_pending(&gpio);
    ok = ((pending & TMS320C6452_GPIO_PIN_MASK(pin_out1)) != 0u);
    report("INTSTAT has falling (bank1)", ok);
    tms320c6452_gpio_clear_pending(&gpio, TMS320C6452_GPIO_PIN_MASK(pin_out1));

    /* Input configuration effect: set pin as input and try to drive it (should not change IN value if externally held).
       Here we only verify that OUT latch updates are not used; we expect writes to SET/CLR to have no effect when DIR=1. */
    const uint32_t pin_in = 10u;
    st = tms320c6452_gpio_set_pin_direction(&gpio, pin_in, true);
    report("configure pin as input", st == TMS320C6452_GPIO_STATUS_OK);
    const uint32_t out_before = tms320c6452_gpio_read_outputs_latch(&gpio);
    (void)tms320c6452_gpio_write_pin(&gpio, pin_in, true);
    const uint32_t out_after = tms320c6452_gpio_read_outputs_latch(&gpio);
    report("SET on input pin has no OUT_DATA effect", out_before == out_after);

    /* Invalid parameter checks */
    st = tms320c6452_gpio_set_pin_direction(&gpio, 32u, false);
    report("invalid pin direction rejected", st == TMS320C6452_GPIO_STATUS_EINVAL);
    st = tms320c6452_gpio_write_pin(&gpio, 33u, true);
    report("invalid pin write rejected", st == TMS320C6452_GPIO_STATUS_EINVAL);

    printf("\nSummary: PASS=%u FAIL=%u\n", g_pass, g_fail);
    return (g_fail == 0u) ? 0 : 1;
}
