#include "TMS320C6452 DSP_core.h"

/* No dynamic init required; provide symbol to force link */
void tms320c6452_gpio_core_link_symbol(void)
{
    (void)gpio_core_get_ops();
}
