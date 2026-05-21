#include <stdint.h>
#include <stdio.h>
#include "tms320c6452_gpio_core.h"
#include "tms320c6452_gpio_banked_architecture_(32_signals,_2_banks).h"

static void test_cb(uint32_t pin, void* ctx)
{
    (void)ctx;
    printf("ISR callback: pin=%u\n", pin);
}

int main(void)
{
    const tms320c6452_gpio_core_api_t* core = tms320c6452_gpio_core_get();
    int rc = tms320c6452_gpio_banked_arch_init(core);
    if (rc != TMS320C6452_GPIO_OK) {
        printf("Init failed: %d\n", rc);
        return rc;
    }

    /* Configure GP3 as output, GP17 as input */
    tms320c6452_gpio_set_outputs(BIT(3));
    tms320c6452_gpio_set_inputs(BIT(17));

    /* Validate DIR register: bit3=0 (output), bit17=1 (input) */
    uint32_t dir = core->read_reg(GPIO_DIR01_OFFSET);
    if ((dir & BIT(3)) != 0) { printf("ERR: GP3 not set as output\n"); return -1; }
    if ((dir & BIT(17)) == 0) { printf("ERR: GP17 not set as input\n"); return -1; }

    /* Drive GP3 high then low using atomic set/clear */
    tms320c6452_gpio_set_pins(BIT(3));
    uint32_t out = core->read_reg(GPIO_OUT_DATA01_OFFSET);
    if ((out & BIT(3)) == 0) { printf("ERR: GP3 not high after set\n"); return -1; }
    tms320c6452_gpio_clear_pins(BIT(3));
    out = core->read_reg(GPIO_OUT_DATA01_OFFSET);
    if ((out & BIT(3)) != 0) { printf("ERR: GP3 not low after clear\n"); return -1; }

    /* Configure interrupt on rising edge for GP17 and enable bank1 interrupts */
    tms320c6452_gpio_enable_rising(BIT(17));
    tms320c6452_gpio_disable_falling(BIT(17));
    tms320c6452_gpio_enable_bank_interrupts(GPIO_BINTEN_EN1);

    /* Register callback for GP17 */
    tms320c6452_gpio_register_callback(17, test_cb, NULL);

    /* Poll for an event (external stimulus required). Timeout after N tries. */
    unsigned tries = 1000000u;
    while (tries--) {
        uint32_t st = tms320c6452_gpio_read_int_status();
        if (st & BIT(17)) {
            /* Simulate ISR dispatch */
            tms320c6452_gpio_isr();
            break;
        }
    }

    /* Edge cases */
    if (tms320c6452_gpio_register_callback(32, test_cb, NULL) == TMS320C6452_GPIO_OK) {
        printf("ERR: accepted invalid pin index\n");
        return -1;
    }

    printf("GPIO banked architecture test completed.\n");
    return 0;
}
