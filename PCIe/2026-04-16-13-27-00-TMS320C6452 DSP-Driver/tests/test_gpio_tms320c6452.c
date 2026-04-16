/*
 * Test application for TMS320C6452 GPIO driver (bare-metal)
 * Validates: PID read, direction set/get, output set/clear, input sampling,
 * interrupt configuration and W1C status using software-generated edges.
 */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../include/gpio_tms320c6452.h"

#ifndef TEST_PIN0
#define TEST_PIN0   (0u)   /* Bank 0, pin 0 */
#endif
#ifndef TEST_PIN1
#define TEST_PIN1   (16u)  /* Bank 1, pin 0 */
#endif

static void delay_cycles(volatile uint32_t n)
{
    while (n-- != 0u) { __asm__ __volatile__("" ::: "memory"); }
}

static void print_result(const char *name, bool pass)
{
    printf("[%-30s] %s\n", name, pass ? "PASS" : "FAIL");
}

/* Provide weak HAL overrides if none supplied by platform */
bool gpio_hal_enable_clock(void)
{
    /* Stub: assume PSC already enabled in this test context */
    return true;
}

bool gpio_hal_configure_pinmux(uint32_t pin)
{
    (void)pin;
    /* Stub: assume pins are already muxed as GPIO */
    return true;
}

int main(void)
{
    printf("TMS320C6452 GPIO Driver Test (big-endian)\n");
#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
    printf("Compiler reports big-endian\n");
#else
    printf("Warning: compiler does not report big-endian; proceeding per spec (GPIO endianness independent)\n");
#endif

    gpio_status_t st = gpio_init(TMS320C6452_GPIO_BASE_ADDR);
    print_result("gpio_init", (st == GPIO_STATUS_OK));

    uint32_t pid = 0u;
    st = gpio_get_pid(&pid);
    bool pid_ok = (st == GPIO_STATUS_OK);
    printf("PID=0x%08X (expected 0x%08X)\n", (unsigned)pid, (unsigned)GPIO_PID_RESET_VALUE);
    /* Do not fail if revision differs; just report */
    print_result("gpio_get_pid", pid_ok);

    /* Configure test pins as outputs */
    st = gpio_set_direction(TEST_PIN0, GPIO_DIR_OUTPUT);
    bool dir0_ok = (st == GPIO_STATUS_OK);
    st = gpio_set_direction(TEST_PIN1, GPIO_DIR_OUTPUT);
    bool dir1_ok = (st == GPIO_STATUS_OK);
    print_result("set_direction pin0", dir0_ok);
    print_result("set_direction pin1", dir1_ok);

    /* Exercise output set/clear and input sampling */
    bool v = false;
    (void)gpio_write_pin(TEST_PIN0, false);
    delay_cycles(1000u);
    (void)gpio_read_pin(TEST_PIN0, &v);
    print_result("pin0 low readback", (v == false));

    (void)gpio_write_pin(TEST_PIN0, true);
    delay_cycles(1000u);
    (void)gpio_read_pin(TEST_PIN0, &v);
    print_result("pin0 high readback", (v == true));

    /* Test bulk mask write: clear pin1, set pin0 */
    (void)gpio_write_mask(GPIO_PIN_MASK(TEST_PIN0), GPIO_PIN_MASK(TEST_PIN1));
    delay_cycles(1000u);
    bool v0=false, v1=false;
    (void)gpio_read_pin(TEST_PIN0, &v0);
    (void)gpio_read_pin(TEST_PIN1, &v1);
    print_result("bulk set pin0", (v0 == true));
    print_result("bulk clr pin1", (v1 == false));

    /* Interrupt configuration test using software-driven edges */
    uint32_t clear_all = GPIO_PIN_MASK(TEST_PIN0) | GPIO_PIN_MASK(TEST_PIN1);
    (void)gpio_irq_clear_status(clear_all);

    /* Enable bank interrupts */
    (void)gpio_irq_bank_enable(0u, true);
    (void)gpio_irq_bank_enable(1u, true);

    /* Rising edge on pin0 */
    (void)gpio_write_pin(TEST_PIN0, false);
    (void)gpio_irq_configure_pin(TEST_PIN0, GPIO_EDGE_RISING);
    (void)gpio_write_pin(TEST_PIN0, true); /* rising */
    delay_cycles(2000u);
    uint32_t pending = 0u;
    (void)gpio_irq_get_status(&pending);
    bool int_pin0 = ((pending & GPIO_PIN_MASK(TEST_PIN0)) != 0u);
    print_result("INTSTAT pin0 rising", int_pin0);
    (void)gpio_irq_clear_status(GPIO_PIN_MASK(TEST_PIN0));

    /* Both edges on pin1 */
    (void)gpio_write_pin(TEST_PIN1, false);
    (void)gpio_irq_configure_pin(TEST_PIN1, GPIO_EDGE_BOTH);
    (void)gpio_write_pin(TEST_PIN1, true); /* rising */
    delay_cycles(1000u);
    (void)gpio_write_pin(TEST_PIN1, false); /* falling */
    delay_cycles(2000u);
    (void)gpio_irq_get_status(&pending);
    bool int_pin1 = ((pending & GPIO_PIN_MASK(TEST_PIN1)) != 0u);
    print_result("INTSTAT pin1 both (any)", int_pin1);
    (void)gpio_irq_clear_status(GPIO_PIN_MASK(TEST_PIN1));

    /* Edge case: invalid pin */
    st = gpio_set_direction(32u, GPIO_DIR_OUTPUT);
    print_result("invalid pin rejected", (st == GPIO_STATUS_INVALID_ARG));

    printf("Test complete.\n");
    return 0;
}
