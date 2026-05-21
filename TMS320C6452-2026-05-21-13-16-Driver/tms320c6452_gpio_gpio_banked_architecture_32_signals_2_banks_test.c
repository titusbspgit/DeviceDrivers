#include <stdint.h>
#include <stdbool.h>
#include "tms320c6452_gpio_gpio_banked_architecture_32_signals_2_banks.h"

/* Simple self-check. Assumes safe environment and that writing GPIO regs is allowed. */

static void test_init_and_edge_enable(void)
{
    tms320c6452_gpio_banked_init();

    /* Enable bank interrupts one by one */
    tms320c6452_gpio_bank_enable_interrupt(0u, true);
    tms320c6452_gpio_bank_enable_interrupt(1u, true);

    /* Enable edges on a sample mask (GPIO0, GPIO16) */
    const uint32_t mask = (1u << 0) | (1u << 16);
    tms320c6452_gpio_enable_rising(mask);
    tms320c6452_gpio_enable_falling(mask);

    /* Read back some states */
    (void)tms320c6452_gpio_read_inputs();
    (void)tms320c6452_gpio_read_outputs();
    (void)tms320c6452_gpio_get_int_status();
}

static void test_atomic_write_mask(void)
{
    /* Drive some outputs high/low atomically (DIR must be set by other configuration) */
    const uint32_t set_mask = (1u << 1) | (1u << 17);
    const uint32_t clr_mask = (1u << 2) | (1u << 18);
    tms320c6452_gpio_write_mask(set_mask, clr_mask);
}

int main(void)
{
    test_init_and_edge_enable();
    test_atomic_write_mask();
    for (;;) {
        /* Idle loop */
    }
    return 0;
}
