#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../include/feature_intstat.h"

static void delay_cycles(volatile uint32_t n){ while(n--){ __asm__ __volatile__("" ::: "memory"); }}

int main(void)
{
    (void)gpio_init(TMS320C6452_GPIO_BASE_ADDR);

    (void)gpio_set_direction(4u, GPIO_DIR_OUTPUT);
    (void)gpio_irq_configure_pin(4u, GPIO_EDGE_BOTH);
    (void)gpio_irq_bank_enable(0u, true);

    (void)gpio_irq_clear_status(0xFFFFFFFFu);

    (void)gpio_write_pin(4u, true);
    (void)gpio_write_pin(4u, false);
    delay_cycles(2000u);

    uint32_t st=0u; (void)gpio_irq_get_status(&st);
    printf("INTSTAT=0x%08X\n", (unsigned)st);
    bool pending = ((st & GPIO_PIN_MASK(4u)) != 0u);
    if (!pending){ printf("FAIL\n"); return 1; }

    (void)gpio_irq_clear_status(GPIO_PIN_MASK(4u));
    st=0u; (void)gpio_irq_get_status(&st);
    bool cleared = ((st & GPIO_PIN_MASK(4u)) == 0u);
    printf("After clear INTSTAT=0x%08X\n", (unsigned)st);
    printf("%s\n", cleared ? "PASS" : "FAIL");
    return cleared ? 0 : 2;
}
