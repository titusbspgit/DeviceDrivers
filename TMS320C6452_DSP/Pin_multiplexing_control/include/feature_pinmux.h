#ifndef FEATURE_PINMUX_H
#define FEATURE_PINMUX_H

#include <stdint.h>
#include <stdbool.h>
#include "../../common/include/gpio_tms320c6452.h"

/* Feature API: enforce pinmux via HAL hook */
static inline bool feature_configure_pin_as_gpio(uint32_t pin)
{
    return gpio_hal_configure_pinmux(pin);
}

#endif /* FEATURE_PINMUX_H */
