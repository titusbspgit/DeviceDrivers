#include "tms320c6452_gpio_pin_multiplexing_control.h"

void tms320c6452_pinmux_apply(const tms320c6452_pinmux_entry_t *table, uint32_t count)
{
    for (uint32_t i = 0; i < count; ++i) {
        tms320c6452_pinmux_write_one(&table[i]);
    }
}

bool tms320c6452_pinmux_verify(const tms320c6452_pinmux_entry_t *table, uint32_t count)
{
    for (uint32_t i = 0; i < count; ++i) {
        if (!tms320c6452_pinmux_is_gpio(&table[i])) {
            return false;
        }
    }
    return true;
}
