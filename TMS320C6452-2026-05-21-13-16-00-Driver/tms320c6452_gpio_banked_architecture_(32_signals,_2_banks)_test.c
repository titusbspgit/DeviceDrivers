#include <stdio.h>
#include <stdint.h>
#include "tms320c6452_gpio_core.h"
#include "tms320c6452_gpio_banked_architecture_(32_signals,_2_banks).h"

static void dump_gpio_registers(const tms320c6452_gpio_core_t *core)
{
    uint32_t pid      = core->read32(core->base, GPIO_PID_OFFSET);
    uint32_t binten   = core->read32(core->base, GPIO_BINTEN_OFFSET);
    uint32_t dir      = core->read32(core->base, GPIO_DIR01_OFFSET);
    uint32_t out      = core->read32(core->base, GPIO_OUT_DATA01_OFFSET);
    uint32_t in       = core->read32(core->base, GPIO_IN_DATA01_OFFSET);
    uint32_t ris_en   = core->read32(core->base, GPIO_SET_RIS_TRIG01_OFFSET);
    uint32_t fal_en   = core->read32(core->base, GPIO_SET_FAL_TRIG01_OFFSET);
    uint32_t intstat  = core->read32(core->base, GPIO_INTSTAT01_OFFSET);

    printf("PID=0x%08X BINTEN=0x%08X DIR=0x%08X OUT=0x%08X IN=0x%08X RISE=0x%08X FALL=0x%08X INTSTAT=0x%08X\n",
           pid, binten, dir, out, in, ris_en, fal_en, intstat);
}

int main(void)
{
    const tms320c6452_gpio_core_t *core = tms320c6452_gpio_core_get();
    tms320c6452_gpio_banked_architecture_32_signals_2_banks_handle_t h;

    printf("TMS320C6452 GPIO banked architecture test start\n");

    if (tms320c6452_gpio_banked_architecture_32_signals_2_banks_init(&h, core) != TMS_GPIO_OK) {
        printf("Init failed\n");
        return -1;
    }

    /* Configure GPIO0 (bank0) and GPIO16 (bank1) as outputs */
    (void)tms320c6452_gpio_banked_architecture_32_signals_2_banks_set_direction(&h, 0u, 0);
    (void)tms320c6452_gpio_banked_architecture_32_signals_2_banks_set_direction(&h, 16u, 0);

    /* Drive them high, then low */
    (void)tms320c6452_gpio_banked_architecture_32_signals_2_banks_write_pin(&h, 0u, 1);
    (void)tms320c6452_gpio_banked_architecture_32_signals_2_banks_write_pin(&h, 16u, 1);

    dump_gpio_registers(core);

    (void)tms320c6452_gpio_banked_architecture_32_signals_2_banks_write_pin(&h, 0u, 0);
    (void)tms320c6452_gpio_banked_architecture_32_signals_2_banks_write_pin(&h, 16u, 0);

    dump_gpio_registers(core);

    /* Configure GPIO1 as input with both-edge interrupt and enable both bank interrupts */
    (void)tms320c6452_gpio_banked_architecture_32_signals_2_banks_set_direction(&h, 1u, 1);
    (void)tms320c6452_gpio_banked_architecture_32_signals_2_banks_configure_pin_interrupt(&h, 1u, 1, 1);
    (void)tms320c6452_gpio_banked_architecture_32_signals_2_banks_enable_bank_interrupt(&h, 0u, 1);
    (void)tms320c6452_gpio_banked_architecture_32_signals_2_banks_enable_bank_interrupt(&h, 1u, 1);

    dump_gpio_registers(core);

    /* Edge cases */
    int ret;
    ret = tms320c6452_gpio_banked_architecture_32_signals_2_banks_set_direction(&h, 32u, 0);
    printf("Set dir invalid pin -> %d (expect %d)\n", ret, TMS_GPIO_ERR_RANGE);

    ret = tms320c6452_gpio_banked_architecture_32_signals_2_banks_write_pin(&h, 1u, 1); /* pin1 is input */
    printf("Write input pin -> %d (expect %d)\n", ret, TMS_GPIO_ERR_INVAL);

    /* Read pin state */
    int val = -1;
    (void)tms320c6452_gpio_banked_architecture_32_signals_2_banks_read_pin(&h, 1u, &val);
    printf("GPIO1 level: %d\n", val);

    /* Clear any pending interrupts observed */
    uint32_t status = tms320c6452_gpio_banked_architecture_32_signals_2_banks_get_int_status(&h);
    if (status) {
        printf("INTSTAT pending: 0x%08X\n", status);
        tms320c6452_gpio_banked_architecture_32_signals_2_banks_clear_int_status(&h, status);
    }

    dump_gpio_registers(core);

    printf("TMS320C6452 GPIO banked architecture test done\n");
    return 0;
}
