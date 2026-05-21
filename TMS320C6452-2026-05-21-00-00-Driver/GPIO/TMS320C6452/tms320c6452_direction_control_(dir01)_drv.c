#include <stdint.h>
#include <stdbool.h>
#include "tms320c6452_direction_control_(dir01).h"

static inline uint32_t rd(uint32_t off) { return tms320c6452_gpio_core.rd(off); }
static inline void     wr(uint32_t off, uint32_t v) { tms320c6452_gpio_core.wr(off, v); }

int tms320c6452_direction_control_init(void)
{
    /* Reset state: all inputs */
    wr(GPIO_DIR01_OFFSET, 0xFFFFFFFFu);
    return 0;
}

void tms320c6452_direction_set_mask(uint32_t dir_mask)
{
    wr(GPIO_DIR01_OFFSET, dir_mask);
}

uint32_t tms320c6452_direction_get_mask(void)
{
    return rd(GPIO_DIR01_OFFSET);
}

int tms320c6452_direction_set_pin(unsigned pin, int is_input)
{
    if (pin >= 32u) { return -1; }
    uint32_t m = rd(GPIO_DIR01_OFFSET);
    if (is_input) {
        m |= (1u << pin);
    } else {
        m &= ~(1u << pin);
    }
    wr(GPIO_DIR01_OFFSET, m);
    return 0;
}
