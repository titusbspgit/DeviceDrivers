#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "tms320c6452_gpio_core.h"
#include "tms320c6452_gpio_banked_architecture_(32_signals,_2_banks).h"

static void dump_regs(const char *tag)
{
    uint32_t dir   = tms320c6452_gpio_core_read(GPIO_DIR01_OFFSET);
    uint32_t out   = tms320c6452_gpio_core_read(GPIO_OUT_DATA01_OFFSET);
    uint32_t in    = tms320c6452_gpio_core_read(GPIO_IN_DATA01_OFFSET);
    uint32_t bint  = tms320c6452_gpio_core_read(GPIO_BINTEN_OFFSET);
    uint32_t istat = tms320c6452_gpio_core_read(GPIO_INTSTAT01_OFFSET);
    printf("%s: DIR=0x%08X OUT=0x%08X IN=0x%08X BINTEN=0x%08X INTSTAT=0x%08X\n",
           tag, dir, out, in, bint, istat);
}

int main(void)
{
    int ret = tms320c6452_gpio_banked_init();
    if (ret != 0) {
        printf("init failed: %d\n", ret);
        return ret;
    }

    dump_regs("after_init");

    /* Configure GP[3:0] as outputs (others remain inputs) */
    uint32_t dir_mask = 0xFFFFFFF0u; /* 1=input, 0=output => bits 3:0 = 0 */
    tms320c6452_gpio_banked_set_direction(dir_mask);

    /* Drive pattern: set 0b0101 (pins 2,0), clear 0b1010 (pins 3,1) */
    tms320c6452_gpio_banked_write(0x00000005u, 0x0000000Au);

    dump_regs("after_output_write");

    /* Validate OUT latch reflects only output pins, inputs unaffected */
    uint32_t out = tms320c6452_gpio_core_read(GPIO_OUT_DATA01_OFFSET);
    if ((out & 0x0000000Fu) != 0x00000005u) {
        printf("FAIL: OUT latch mismatch on low nibble: 0x%08X\n", out);
    } else {
        printf("PASS: OUT latch low nibble = 0x%X\n", out & 0xF);
    }

    /* Attempt to write to an input-configured pin (bit 4). Expect no change to OUT latch bit 4. */
    uint32_t out_before = tms320c6452_gpio_core_read(GPIO_OUT_DATA01_OFFSET);
    tms320c6452_gpio_banked_write(0x00000010u, 0x00000000u); /* set bit 4 (configured as input) */
    uint32_t out_after  = tms320c6452_gpio_core_read(GPIO_OUT_DATA01_OFFSET);
    if (((out_before ^ out_after) & 0x00000010u) != 0u) {
        printf("FAIL: OUT latch changed for input pin 4 (0x%08X -> 0x%08X)\n", out_before, out_after);
    } else {
        printf("PASS: OUT latch unchanged for input pin 4\n");
    }

    /* Configure edges on inputs: rising on bit 4, falling on bit 5 */
    tms320c6452_gpio_banked_config_edge(0x00000010u, 0x00000000u, 0x00000020u, 0x00000000u);

    /* Enable bank 0 interrupt */
    tms320c6452_gpio_banked_enable_bank_irq(0u, 1);

    dump_regs("after_irq_cfg");

    /* Read inputs */
    uint32_t in = tms320c6452_gpio_banked_read_inputs();
    printf("IN=0x%08X\n", in);

    /* Invoke ISR manually (no real event in this test) */
    tms320c6452_gpio_banked_isr();

    printf("Test complete.\n");
    return 0;
}
