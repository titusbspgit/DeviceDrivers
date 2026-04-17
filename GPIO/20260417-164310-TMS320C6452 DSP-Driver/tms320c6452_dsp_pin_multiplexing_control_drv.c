#include "tms320c6452_dsp_pin_multiplexing_control.h"

bool tms320c6452_dsp_pin_multiplexing_control_validate(uint32_t pin)
{
    return (pin < 32u);
}

bool tms320c6452_dsp_pin_multiplexing_control_configure_gpio(uint32_t pin)
{
    (void)pin;
    /* UNKNOWN pinmux registers; cannot configure */
    return false;
}

int tms320c6452_dsp_pin_multiplexing_control_query_function(uint32_t pin)
{
    (void)pin;
    /* UNKNOWN pin function */
    return -1;
}
