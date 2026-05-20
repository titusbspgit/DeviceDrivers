#include <stdio.h>
#include <stdint.h>
#include "tms320c6452_gpio_banked_architecture_32_signals_2_banks_drv.h"

static void dump_bank(const char *tag, uint8_t bank)
{
    uint16_t dir  = tms320c6452_gpio_banked_arch_bank_read_dir(bank);
    uint16_t in   = tms320c6452_gpio_banked_arch_bank_read_input(bank);
    uint16_t out  = tms320c6452_gpio_banked_arch_bank_read_output(bank);
    printf("%s bank%u: DIR=0x%04X (1=in), IN=0x%04X, OUT=0x%04X\n", tag, bank, dir, in, out);
}

static void bank0_cb(uint16_t status, void *ctx)
{
    (void)ctx;
    printf("BANK0 ISR: status=0x%04X\n", status);
    (void)tms320c6452_gpio_banked_arch_edge_ack(TMS320C6452_GPIO_BANK0, status);
}

int main(void)
{
    int rc = tms320c6452_gpio_banked_arch_init();
    if (rc != 0) {
        printf("init failed: %d\n", rc);
        return rc;
    }

    rc = tms320c6452_gpio_banked_arch_register_isr(TMS320C6452_GPIO_BANK0, bank0_cb, NULL);
    if (rc != 0) { printf("register_isr b0 failed: %d\n", rc); return rc; }

    uint16_t dir_b0 = 0xFFFFu;
    dir_b0 &= (uint16_t)~0x000Fu; /* pins 0..3 outputs */
    rc = tms320c6452_gpio_banked_arch_set_bank_dir(TMS320C6452_GPIO_BANK0, dir_b0);
    if (rc != 0) { printf("set_bank_dir b0 failed: %d\n", rc); return rc; }

    uint16_t rd_dir_b0 = tms320c6452_gpio_banked_arch_bank_read_dir(TMS320C6452_GPIO_BANK0);
    printf("DIR bank0 expected=0x%04X got=0x%04X\n", dir_b0, rd_dir_b0);

    rc = tms320c6452_gpio_banked_arch_bank_write_out_mask(TMS320C6452_GPIO_BANK0, 0x0005u, 0x000Fu);
    if (rc != 0) { printf("write_out_mask b0 failed: %d\n", rc); return rc; }

    dump_bank("After OUT mask write", TMS320C6452_GPIO_BANK0);

    rc = tms320c6452_gpio_banked_arch_bank_set_bits(TMS320C6452_GPIO_BANK0, 0x0008u);
    if (rc != 0) { printf("set_bits b0 failed: %d\n", rc); return rc; }
    rc = tms320c6452_gpio_banked_arch_bank_clear_bits(TMS320C6452_GPIO_BANK0, 0x0002u);
    if (rc != 0) { printf("clear_bits b0 failed: %d\n", rc); return rc; }

    dump_bank("After SET/CLR", TMS320C6452_GPIO_BANK0);

    rc = tms320c6452_gpio_banked_arch_bank_enable_rising(TMS320C6452_GPIO_BANK0, 0x0003u);
    if (rc != 0) { printf("enable_rising b0 failed: %d\n", rc); return rc; }
    rc = tms320c6452_gpio_banked_arch_bank_enable_falling(TMS320C6452_GPIO_BANK0, 0x0003u);
    if (rc != 0) { printf("enable_falling b0 failed: %d\n", rc); return rc; }

    rc = tms320c6452_gpio_banked_arch_enable_bank_interrupt(TMS320C6452_GPIO_BANK0);
    if (rc != 0) { printf("enable_bank_interrupt b0 failed: %d\n", rc); return rc; }

    uint32_t isr = tms320c6452_gpio_banked_arch_get_int_status();
    printf("INTSTAT01=0x%08X\n", isr);

    tms320c6452_gpio_banked_arch_bank0_isr();

    rc = tms320c6452_gpio_banked_arch_set_bank_dir(2u, 0xFFFFu);
    printf("set_bank_dir invalid bank rc=%d (expect error)\n", rc);
    rc = tms320c6452_gpio_banked_arch_register_isr(2u, bank0_cb, NULL);
    printf("register_isr invalid bank rc=%d (expect error)\n", rc);

    (void)tms320c6452_gpio_banked_arch_bank_disable_rising(TMS320C6452_GPIO_BANK0, 0x0003u);
    (void)tms320c6452_gpio_banked_arch_bank_disable_falling(TMS320C6452_GPIO_BANK0, 0x0003u);
    (void)tms320c6452_gpio_banked_arch_disable_bank_interrupt(TMS320C6452_GPIO_BANK0);

    dump_bank("Final", TMS320C6452_GPIO_BANK0);

    return 0;
}
