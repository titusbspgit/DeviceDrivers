#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "tms320c6452_gpio_banked_architecture_32_signals_2_banks.h"

/* Simple callback to observe ISR dispatch */
static void test_gpio_cb(uint32_t pin)
{
    printf("ISR callback for pin %u\n", (unsigned)pin);
}

static void dump_regs(void)
{
    printf("DIR01=0x%08X OUT=0x%08X SET=0x%08X CLR=0x%08X IN=0x%08X\n",
           (unsigned)TMS320C6452_GPIO_DIR01,
           (unsigned)TMS320C6452_GPIO_OUT_DATA01,
           (unsigned)TMS320C6452_GPIO_SET_DATA01,
           (unsigned)TMS320C6452_GPIO_CLR_DATA01,
           (unsigned)TMS320C6452_GPIO_IN_DATA01);
    printf("RIS_SET=0x%08X RIS_CLR=0x%08X FAL_SET=0x%08X FAL_CLR=0x%08X INTSTAT=0x%08X BINTEN=0x%08X\n",
           (unsigned)TMS320C6452_GPIO_SET_RIS_TRIG01,
           (unsigned)TMS320C6452_GPIO_CLR_RIS_TRIG01,
           (unsigned)TMS320C6452_GPIO_SET_FAL_TRIG01,
           (unsigned)TMS320C6452_GPIO_CLR_FAL_TRIG01,
           (unsigned)TMS320C6452_GPIO_INTSTAT01,
           (unsigned)TMS320C6452_GPIO_BINTEN);
}

/* Edge cases: invalid parameters */
static void run_edge_cases(void)
{
    int rc;
    bool dir;
    uint32_t v;
    uint16_t bankv;

    rc = tms320c6452_gpio_banked_set_direction(32u, true);  /* invalid pin */
    printf("set_direction(pin=32) rc=%d (expect -1)\n", rc);

    rc = tms320c6452_gpio_banked_get_direction(32u, &dir);
    printf("get_direction(pin=32) rc=%d (expect -1)\n", rc);

    rc = tms320c6452_gpio_banked_set_pin(32u);
    printf("set_pin(pin=32) rc=%d (expect -1)\n", rc);

    rc = tms320c6452_gpio_banked_clear_pin(32u);
    printf("clear_pin(pin=32) rc=%d (expect -1)\n", rc);

    rc = tms320c6452_gpio_banked_write_pin(32u, 1u);
    printf("write_pin(pin=32) rc=%d (expect -1)\n", rc);

    rc = tms320c6452_gpio_banked_read_pin(32u, &v);
    printf("read_pin(pin=32) rc=%d (expect -1)\n", rc);

    rc = tms320c6452_gpio_banked_read_bank_inputs(2u, &bankv);
    printf("read_bank_inputs(bank=2) rc=%d (expect -1)\n", rc);

    rc = tms320c6452_gpio_banked_write_bank_mask(2u, 0xFFFFu, 0x0000u);
    printf("write_bank_mask(bank=2) rc=%d (expect -1)\n", rc);

    rc = tms320c6452_gpio_banked_enable_bank_irq(2u, true);
    printf("enable_bank_irq(bank=2) rc=%d (expect -1)\n", rc);

    rc = tms320c6452_gpio_banked_configure_pin_irq(32u, true, false);
    printf("configure_pin_irq(pin=32) rc=%d (expect -1)\n", rc);
}

int main(void)
{
    printf("TMS320C6452 GPIO banked architecture test start\n");

    /* Initialize */
    tms320c6452_gpio_banked_init();
    dump_regs();

    /* Configure pin 0 as output and toggle */
    int rc = tms320c6452_gpio_banked_set_direction(0u, false); /* output */
    printf("set_direction(pin0, output) rc=%d\n", rc);
    dump_regs();

    rc = tms320c6452_gpio_banked_set_pin(0u);
    printf("set_pin(0) rc=%d\n", rc);
    dump_regs();

    rc = tms320C6452_gpio_banked_clear_pin(0u); /* Intentional case-mismatch removed in production; kept here would fail. */
    /* Fix: use correct symbol: */
    rc = tms320c6452_gpio_banked_clear_pin(0u);
    printf("clear_pin(0) rc=%d\n", rc);
    dump_regs();

    /* Configure pin 1 as input and read */
    rc = tms320c6452_gpio_banked_set_direction(1u, true); /* input */
    printf("set_direction(pin1, input) rc=%d\n", rc);
    uint32_t pin1_val = 0u;
    rc = tms320c6452_gpio_banked_read_pin(1u, &pin1_val);
    printf("read_pin(1) rc=%d value=%u\n", rc, (unsigned)pin1_val);

    /* Bank 0 write: set pin 2 and clear pin 3 via masks */
    rc = tms320c6452_gpio_banked_set_direction(2u, false); /* output */
    rc |= tms320c6452_gpio_banked_set_direction(3u, false); /* output */
    rc |= tms320c6452_gpio_banked_write_bank_mask(0u, (uint16_t)(1u << 2), (uint16_t)(1u << 3));
    printf("write_bank_mask(bank0, set p2, clr p3) rc=%d\n", rc);
    dump_regs();

    /* Read bank inputs for both banks */
    uint16_t bank0 = 0u, bank1 = 0u;
    rc = tms320c6452_gpio_banked_read_bank_inputs(0u, &bank0);
    rc |= tms320c6452_gpio_banked_read_bank_inputs(1u, &bank1);
    printf("bank0_in=0x%04X bank1_in=0x%04X rc=%d\n", bank0, bank1, rc);

    /* Interrupt configuration: enable bank 0, configure pin 4 rising edge */
    rc = tms320c6452_gpio_banked_enable_bank_irq(0u, true);
    printf("enable_bank_irq(bank0) rc=%d\n", rc);
    rc = tms320c6452_gpio_banked_register_callback(4u, test_gpio_cb);
    printf("register_callback(pin4) rc=%d\n", rc);
    rc = tms320c6452_gpio_banked_configure_pin_irq(4u, true, false);
    printf("configure_pin_irq(pin4 rising) rc=%d\n", rc);
    dump_regs();

    /* Simulate callback path (cannot set INTSTAT in SW) */
    test_gpio_cb(4u);

    /* Clear any (potential) pending flags */
    tms320c6452_gpio_banked_clear_pending((uint32_t)(1u << 4));
    dump_regs();

    /* Edge cases */
    run_edge_cases();

    printf("TMS320C6452 GPIO banked architecture test complete\n");
    return 0;
}
