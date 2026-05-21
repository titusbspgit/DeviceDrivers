#include <stdint.h>
#include <stdbool.h>
#include "tms320c6452_gpio_core.h"
#include "tms320c6452_gpio_banked_architecture_(32_signals,_2_banks).h"

/* Internal helpers */
static inline uint32_t rd(uint32_t off) { return tms320c6452_gpio_core.rd(off); }
static inline void wr(uint32_t off, uint32_t v) { tms320c6452_gpio_core.wr(off, v); }

int tms320c6452_gpio_banked_init(void)
{
    /* Disable bank interrupts */
    wr(GPIO_BINTEN_OFFSET, 0u);

    /* Clear all rising/falling edge triggers */
    wr(GPIO_CLR_RIS_TRIG01_OFFSET, 0xFFFFFFFFu);
    wr(GPIO_CLR_FAL_TRIG01_OFFSET, 0xFFFFFFFFu);

    /* Set all pins to input (DIR=1) */
    wr(GPIO_DIR01_OFFSET, 0xFFFFFFFFu);

    /* Default outputs low via clear (harmless on inputs) */
    wr(GPIO_CLR_DATA01_OFFSET, 0xFFFFFFFFu);

    return 0;
}

void tms320c6452_gpio_banked_set_direction(uint32_t dir_mask)
{
    /* 1=input, 0=output */
    wr(GPIO_DIR01_OFFSET, dir_mask);
}

void tms320c6452_gpio_banked_write(uint32_t set_mask, uint32_t clr_mask)
{
    if (set_mask) { wr(GPIO_SET_DATA01_OFFSET, set_mask); }
    if (clr_mask) { wr(GPIO_CLR_DATA01_OFFSET, clr_mask); }
}

uint32_t tms320c6452_gpio_banked_read_inputs(void)
{
    return rd(GPIO_IN_DATA01_OFFSET);
}

void tms320c6452_gpio_banked_config_edge(uint32_t ris_enable_mask,
                                         uint32_t ris_disable_mask,
                                         uint32_t fal_enable_mask,
                                         uint32_t fal_disable_mask)
{
    if (ris_enable_mask)  { wr(GPIO_SET_RIS_TRIG01_OFFSET, ris_enable_mask); }
    if (ris_disable_mask) { wr(GPIO_CLR_RIS_TRIG01_OFFSET, ris_disable_mask); }
    if (fal_enable_mask)  { wr(GPIO_SET_FAL_TRIG01_OFFSET, fal_enable_mask); }
    if (fal_disable_mask) { wr(GPIO_CLR_FAL_TRIG01_OFFSET, fal_disable_mask); }
}

void tms320c6452_gpio_banked_enable_bank_irq(unsigned bank, int enable)
{
    uint32_t binten = rd(GPIO_BINTEN_OFFSET);
    switch (bank) {
    case 0:
        if (enable) binten |= GPIO_BINTEN_EN0; else binten &= ~GPIO_BINTEN_EN0;
        break;
    case 1:
        if (enable) binten |= GPIO_BINTEN_EN1; else binten &= ~GPIO_BINTEN_EN1;
        break;
    default:
        /* Invalid bank: ignore deterministically */
        return;
    }
    wr(GPIO_BINTEN_OFFSET, binten & (GPIO_BINTEN_EN0 | GPIO_BINTEN_EN1));
}

uint32_t tms320c6452_gpio_banked_get_int_status(void)
{
    return rd(GPIO_INTSTAT01_OFFSET);
}

__attribute__((weak)) void tms320c6452_gpio_banked_isr_callback(uint32_t pending_map)
{
    (void)pending_map; /* default weak stub: user may override */
}

void tms320c6452_gpio_banked_isr(void)
{
    /* Read pending status across both banks */
    uint32_t pending = rd(GPIO_INTSTAT01_OFFSET);
    if (pending != 0u) {
        tms320c6452_gpio_banked_isr_callback(pending);
    }
    /* Clearing of CPU interrupt flag is outside GPIO, done in platform IRQ controller. */
}
