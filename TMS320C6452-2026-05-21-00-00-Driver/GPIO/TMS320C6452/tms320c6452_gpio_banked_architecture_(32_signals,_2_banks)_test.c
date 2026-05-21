#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "tms320c6452_gpio_core.h"
#include "tms320c6452_gpio_banked_architecture_(32_signals,_2_banks).h"

static void dump_regs(const char *tag)
{
    uint32_t dir   = tms320c6452_gpio_core.rd(GPIO_DIR01_OFFSET);
    uint32_t out   = tms320c6452_gpio_core.rd(GPIO_OUT_DATA01_OFFSET);
    uint32_t in    = tms320c6452_gpio_core.rd(GPIO_IN_DATA01_OFFSET);
    uint32_t bint  = tms320c6452_gpio_core.rd(GPIO_BINTEN_OFFSET);
    uint32_t istat = tms320c6452_gpio_core.rd(GPIO_INTSTAT01_OFFSET);
    printf("%s: DIR=0x%08X OUT=0x%08X IN=0x%08X BINTEN=0x%08X INTSTAT=0x%08X\n",
           tag, dir, out, in, bint, istat);
}

/* Optional ISR callback override for test visibility */
void tms320c6452_gpio_banked_isr_callback(uint32_t pending_map)
{
    printf("ISR pending bitmap: 0x%08X\n", pending_map);
}

int main(void)
{
    /* Init */
    int ret = tms320c6452_gpio_banked_init();
    if (ret != 0) {
        printf("init failed: %d\n", ret);
        return ret;
    }
    dump_regs("after_init");

    /* Configure GP[3:0] as outputs (others inputs) */
    uint32_t dir_mask = 0xFFFFFFF0u; /* 1=input, 0=output => lower 4 bits as outputs */
    tms320c6452_gpio_banked_set_direction(dir_mask);

    /* Trigger: set bits 2,0; clear bits 3,1 */
    tms320c6452_gpio_banked_write(0x00000005u, 0x0000000Au);
    dump_regs("after_output_write");

    /* Verify OUT latch for low nibble */
    uint32_t out = tms320c6452_gpio_core.rd(GPIO_OUT_DATA01_OFFSET);
    if ((out & 0xFu) != 0x5u) {
        printf("FAIL: OUT low nibble exp=0x5 got=0x%X\n", out & 0xFu);
    } else {
        printf("PASS: OUT low nibble = 0x%X\n", out & 0xFu);
    }

    /* Read inputs snapshot */
    uint32_t in = tms320c6452_gpio_banked_read_inputs();
    printf("Inputs: 0x%08X\n", in);

    /* Manually invoke ISR (no real hardware event in this test) */
    tms320c6452_gpio_banked_isr();

    printf("Test complete.\n");
    return 0;
}
