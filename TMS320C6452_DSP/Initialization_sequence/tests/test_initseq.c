#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../include/feature_initseq.h"

/* Override HALs for visibility */
bool gpio_hal_enable_clock(void){ return true; }
bool gpio_hal_configure_pinmux(uint32_t pin){ (void)pin; return true; }

int main(void)
{
    gpio_status_t st = gpio_init(TMS320C6452_GPIO_BASE_ADDR);
    if (st != GPIO_STATUS_OK){ printf("FAIL init=%u\n", (unsigned)st); return 1; }

    (void)gpio_set_direction(0u, GPIO_DIR_OUTPUT);
    (void)gpio_write_pin(0u, false);
    (void)gpio_irq_clear_status(0xFFFFFFFFu);

    printf("PASS\n");
    return 0;
}
