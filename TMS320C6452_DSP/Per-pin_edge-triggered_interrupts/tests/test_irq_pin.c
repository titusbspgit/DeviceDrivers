#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../include/feature_irq_pin.h"

static void delay_cycles(volatile uint32_t n){ while(n--){ __asm__ __volatile__("" ::: "memory"); }}

int main(void)
{
    (void)gpio_init(TMS320C6452_GPIO_BASE_ADDR);

    (void)gpio_set_direction(3u, GPIO_DIR_OUTPUT);

    (void)gpio_irq_configure_pin(3u, GPIO_EDGE_RISING);
    (void)gpio_irq_bank_enable(0u, true);

    (void)gpio_irq_clear_status(0xFFFFFFFFu);

    (void)gpio_write_pin(3u, false);
    (void)gpio_write_pin(3u, true); /* rising edge */
    delay_cycles(2000u);

    uint32_t pending=0u; (void)gpio_irq_get_status(&pending);
    bool ok = ((pending & GPIO_PIN_MASK(3u)) != 0u);
    printf("irq pin3 rising pending=0x%08X\n", (unsigned)pending);
    printf("%s\n", ok ? "PASS" : "FAIL");

    (void)gpio_irq_clear_status(GPIO_PIN_MASK(3u));
    return ok ? 0 : 1;
}
