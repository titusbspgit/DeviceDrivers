#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "tms320c6452_gpio_banked_architecture_(32_signals,_2_banks)_drv.h"

static void print_bits32(const char *name, uint32_t v)
{
    printf("%s = 0x%08X\n", name, (unsigned)v);
}

int main(void)
{
    int rc;
    uint32_t v;
    bool b;

    rc = tms320c6452_gpio_banked_architecture_init();
    printf("init rc=%d\n", rc);

    rc = tms320c6452_gpio_set_dir(0u, false);
    printf("set_dir(0,out) rc=%d\n", rc);
    rc = tms320c6452_gpio_write(0u, true);
    printf("write(0,1) rc=%d\n", rc);
    rc = tms320c6452_gpio_write(0u, false);
    printf("write(0,0) rc=%d\n", rc);

    rc = tms320c6452_gpio_set_dir(16u, false);
    printf("set_dir(16,out) rc=%d\n", rc);
    rc = tms320c6452_gpio_write(16u, true);
    printf("write(16,1) rc=%d\n", rc);
    rc = tms320c6452_gpio_write(16u, false);
    printf("write(16,0) rc=%d\n", rc);

    rc = tms320c6452_gpio_get_dir(0u, &b);
    printf("get_dir(0) rc=%d val=%u\n", rc, (unsigned)b);

    rc = tms320c6452_gpio_set_dir(0u, true);
    printf("set_dir(0,in) rc=%d\n", rc);
    rc = tms320c6452_gpio_read(0u, &b);
    printf("read(0) rc=%d val=%u\n", rc, (unsigned)b);

    rc = tms320c6452_gpio_enable_edge(0u, true, true);
    printf("enable_edge(0,ris,fall) rc=%d\n", rc);

    rc = tms320c6452_gpio_bank_int_enable(0u, true);
    printf("bank_int_enable(0,1) rc=%d\n", rc);

    v = tms320c6452_gpio_int_status();
    print_bits32("INTSTAT01", v);

    rc = tms320c6452_gpio_set_dir(32u, false);
    printf("set_dir(32,out) rc=%d (expect error)\n", rc);

    rc = tms320c6452_gpio_bank_int_enable(2u, true);
    printf("bank_int_enable(2,1) rc=%d (expect error)\n", rc);

    v = *(volatile uint32_t *)((uintptr_t)TMS320C6452_GPIO_BASE + GPIO_DIR01_OFS);
    print_bits32("DIR01", v);
    v = *(volatile uint32_t *)((uintptr_t)TMS320C6452_GPIO_BASE + GPIO_OUT_DATA01_OFS);
    print_bits32("OUT_DATA01", v);
    v = *(volatile uint32_t *)((uintptr_t)TMS320C6452_GPIO_BASE + GPIO_IN_DATA01_OFS);
    print_bits32("IN_DATA01", v);

    return 0;
}
