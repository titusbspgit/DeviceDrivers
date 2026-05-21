#include <stdint.h>
#include <stdbool.h>
#include "tms320c6452_gpio_direction_control_dir01.h"

int main(void)
{
    /* Initialize explicitly to inputs */
    tms320c6452_gpio_direction_control_init_all_inputs();

    /* Configure a couple of pins as outputs */
    tms320c6452_gpio_set_dir_output(0u);
    tms320c6452_gpio_set_dir_output(16u);

    /* Revert one pin back to input */
    tms320c6452_gpio_set_dir_input(16u);

    /* Read back DIR register */
    volatile uint32_t dir = tms320c6452_gpio_read_dir();
    (void)dir;

    for (;;) {
        /* Idle */
    }

    return 0;
}
