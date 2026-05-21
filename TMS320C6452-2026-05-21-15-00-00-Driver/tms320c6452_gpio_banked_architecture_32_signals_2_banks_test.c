#include <stdio.h>
#include <stdint.h>
#include "tms320c6452_gpio_core.h"
#include "tms320c6452_gpio_banked_architecture_32_signals_2_banks.h"

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

    if (tms320c6452_gpio_banked_architecture_32_signals_2_banks_init(&h, core) != TMS_GPIO_OK) {
        return -1;
    }

    (void)tms320c6452_gpio_banked_architecture_32_signals_2_banks_set_direction(&h, 0u, 0);
    (void)tms320c6452_gpio_banked_architecture_32_signals_2_banks_set_direction(&h, 16u, 0);

    (void)tms320c6452_gpio_banked_architecture_32_signals_2_banks_write_pin(&h, 0u, 1);
    (void)tms320c6452_gpio_banked_architecture_32_signals_2_banks_write_pin(&h, 16u, 1);

    dump_gpio_registers(core);

    (void)tms320c6452_gpio_banked_architecture_32_signals_2_banks_write_pin(&h, 0u, 0);
    (void)tms320c6452_gpio_banked_architecture_32_signals_2_banks_write_pin(&h, 16u, 0);

    dump_gpio_registers(core);

    (void)tms320c6452_gpio_banked_architecture_32_signals_2_banks_set_direction(&h, 1u, 1);
    (void)tms320c6452_gpio_banked_architecture_32_signals_2_banks_configure_pin_interrupt(&h, 1u, 1, 1);
    (void)tms320c6452_gpio_banked_architecture_32_signals_2_banks_enable_bank_interrupt(&h, 0u, 1);
    (void)tms320c6452_gpio_banked_architecture_32_signals_2_banks_enable_bank_interrupt(&h, 1u, 1);

    dump_gpio_registers(core);

    int ret;
    ret = tms320c6452_gpio_banked_architecture_32_signals_2_banks_set_direction(&h, 32u, 0);
    printf("Set dir invalid pin -> %d\n", ret);

    ret = tms320c6452_gpio_banked_architecture_32_signals_2_banks_write_pin(&h, 1u, 1);
    printf("Write input pin -> %d\n", ret);

    int val = -1;
    (void)tms320c6452_gpio_banked_architecture_32_signals_2_banks_read_pin(&h, 1u, &val);
    printf("GPIO1 level: %d\n", val);

    uint32_t status = tms320c6452_gpio_banked_architecture_32_signals_2_banks_get_int_status(&h);
    if (status) {
        tms320c6452_gpio_banked_architecture_32_signals_2_banks_clear_int_status(&h, status);
    }

    dump_gpio_registers(core);
    return 0;
}
