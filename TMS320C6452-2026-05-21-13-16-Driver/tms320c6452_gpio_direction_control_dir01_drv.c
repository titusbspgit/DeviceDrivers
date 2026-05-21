#include "tms320c6452_gpio_direction_control_dir01.h"

/* No non-inline APIs required; direction control provided as inline helpers for deterministic register writes. */

void tms320c6452_gpio_direction_control_init_all_inputs(void)
{
    /* Default after reset is inputs, but write explicitly for determinism */
    HWREG32(GPIO_DIR01) = 0xFFFFFFFFu;
}
