#ifndef FEATURE_GPIO_BANKED_ARCHITECTURE_H
#define FEATURE_GPIO_BANKED_ARCHITECTURE_H

#include <stdint.h>
#include <stdbool.h>
#include "../../common/include/gpio_tms320c6452.h"

/* Feature-specific helper: return bank mask for a bank index */
static inline uint32_t gpio_feature_bank_mask(uint32_t bank)
{
    return (bank == 0u) ? 0x0000FFFFu : 0xFFFF0000u;
}

#endif /* FEATURE_GPIO_BANKED_ARCHITECTURE_H */
