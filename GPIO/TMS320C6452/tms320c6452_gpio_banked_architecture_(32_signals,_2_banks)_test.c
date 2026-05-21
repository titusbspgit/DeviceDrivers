#include <stdint.h>
#include <stdbool.h>
#include "tms320c6452_gpio_banked_architecture_(32_signals,_2_banks).h"

/* Simple test macros (no stdio) */
static volatile int test_errors = 0;
#define TEST_ASSERT(x) do { if (!(x)) { test_errors++; } } while (0)

static void test_gpio_banked_architecture(void)
{
    tms320c6452_gpio_core_t core;
    tms320c6452_gpio_core_init(&core, TMS320C6452_GPIO_BASE);

    /* Clear stale interrupts */
    tms320c6452_gpio_banked_clear_all_interrupts(&core);

    /* Configure GPIO0 and GPIO17 as outputs and toggle */
    TEST_ASSERT(tms320c6452_gpio_banked_set_dir(&core, 0u, false) == 0);
    TEST_ASSERT(tms320c6452_gpio_banked_set_dir(&core, 17u, false) == 0);

    TEST_ASSERT(tms320c6452_gpio_banked_get_dir(&core, 0u) == 0);
    TEST_ASSERT(tms320c6452_gpio_banked_get_dir(&core, 17u) == 0);

    TEST_ASSERT(tms320c6452_gpio_banked_write_pin(&core, 0u, true) == 0);
    TEST_ASSERT(tms320c6452_gpio_banked_write_pin(&core, 17u, true) == 0);
    TEST_ASSERT(tms320c6452_gpio_banked_write_pin(&core, 0u, false) == 0);
    TEST_ASSERT(tms320c6452_gpio_banked_write_pin(&core, 17u, false) == 0);

    /* Configure GPIO1 as input and read level */
    TEST_ASSERT(tms320c6452_gpio_banked_set_dir(&core, 1u, true) == 0);
    int lvl = tms320c6452_gpio_banked_read_pin(&core, 1u);
    TEST_ASSERT(lvl == 0 || lvl == 1);

    /* Configure edges on GPIO1, enable Bank0 interrupt */
    TEST_ASSERT(tms320c6452_gpio_banked_config_edge(&core, 1u, true, true) == 0);
    TEST_ASSERT(tms320c6452_gpio_banked_enable_bank_irq(&core, GPIO_BANK0, true) == 0);

    /* Read/clear INTSTAT (cannot force hardware edges here) */
    (void)tms320c6452_gpio_banked_get_intstatus(&core);
    tms320c6452_gpio_banked_clear_intstatus(&core, 0x00000000u); /* no-op clear */

    /* Edge cases: invalid pin and bank */
    TEST_ASSERT(tms320c6452_gpio_banked_set_dir(&core, 32u, true) < 0);
    TEST_ASSERT(tms320c6452_gpio_banked_write_pin(&core, 33u, true) < 0);
    TEST_ASSERT(tms320c6452_gpio_banked_enable_bank_irq(&core, 2u, true) < 0);
}

int main(void)
{
    test_gpio_banked_architecture();
    /* Return number of errors as exit code (0 = success) */
    return test_errors;
}
