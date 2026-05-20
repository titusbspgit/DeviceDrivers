#include <stdint.h>
#include "tms320c6452_gpio_banked_architecture_(32_signals,_2_banks)_drv.h"

/* MMIO helpers */
static inline volatile uint32_t *gpio_reg(uint32_t ofs)
{
    return (volatile uint32_t *)((uintptr_t)TMS320C6452_GPIO_BASE + ofs);
}

static inline uint32_t mmio_read32(uint32_t ofs)
{
    volatile uint32_t *r = gpio_reg(ofs);
    uint32_t v = *r;
    __asm__ __volatile__("" ::: "memory");
    return v;
}

static inline void mmio_write32(uint32_t ofs, uint32_t v)
{
    __asm__ __volatile__("" ::: "memory");
    *gpio_reg(ofs) = v;
    __asm__ __volatile__("" ::: "memory");
}

static inline void mmio_setbits32(uint32_t ofs, uint32_t mask)
{
    uint32_t v = mmio_read32(ofs);
    v |= mask;
    mmio_write32(ofs, v);
}

static inline void mmio_clrbits32(uint32_t ofs, uint32_t mask)
{
    uint32_t v = mmio_read32(ofs);
    v &= ~mask;
    mmio_write32(ofs, v);
}

/* Internal validation */
static inline int valid_pin(uint32_t pin) { return (pin < TMS320C6452_GPIO_PIN_COUNT) ? 1 : 0; }
static inline int valid_bank(uint32_t bank) { return (bank < TMS320C6452_GPIO_BANK_COUNT) ? 1 : 0; }

int tms320c6452_gpio_banked_architecture_init(void)
{
    /* Deterministic: disable all bank interrupts, disable all per-pin edges */
    mmio_write32(GPIO_BINTEN_OFS, TMS320C6452_GPIO_BINTEN_RESET);
    mmio_write32(GPIO_CLR_RIS_TRIG01_OFS, 0xFFFFFFFFu);
    mmio_write32(GPIO_CLR_FAL_TRIG01_OFS, 0xFFFFFFFFu);
    return TMS320C6452_GPIO_OK;
}

int tms320c6452_gpio_set_dir(uint32_t pin, bool input)
{
    if (!valid_pin(pin)) return TMS320C6452_GPIO_ERR_INVAL;
    if (input) {
        mmio_setbits32(GPIO_DIR01_OFS, GPIO_BIT(pin));
    } else {
        mmio_clrbits32(GPIO_DIR01_OFS, GPIO_BIT(pin));
    }
    return TMS320C6452_GPIO_OK;
}

int tms320c6452_gpio_get_dir(uint32_t pin, bool *input)
{
    if (!valid_pin(pin) || input == (void*)0) return TMS320C6452_GPIO_ERR_INVAL;
    uint32_t dir = mmio_read32(GPIO_DIR01_OFS);
    *input = ((dir & GPIO_BIT(pin)) != 0u);
    return TMS320C6452_GPIO_OK;
}

int tms320c6452_gpio_write(uint32_t pin, bool high)
{
    if (!valid_pin(pin)) return TMS320C6452_GPIO_ERR_INVAL;
    if (high) {
        mmio_write32(GPIO_SET_DATA01_OFS, GPIO_BIT(pin));
    } else {
        mmio_write32(GPIO_CLR_DATA01_OFS, GPIO_BIT(pin));
    }
    return TMS320C6452_GPIO_OK;
}

int tms320c6452_gpio_read(uint32_t pin, bool *high)
{
    if (!valid_pin(pin) || high == (void*)0) return TMS320C6452_GPIO_ERR_INVAL;
    uint32_t v = mmio_read32(GPIO_IN_DATA01_OFS);
    *high = ((v & GPIO_BIT(pin)) != 0u);
    return TMS320C6452_GPIO_OK;
}

int tms320c6452_gpio_enable_edge(uint32_t pin, bool rising, bool falling)
{
    if (!valid_pin(pin)) return TMS320C6452_GPIO_ERR_INVAL;
    if (rising)  mmio_write32(GPIO_SET_RIS_TRIG01_OFS, GPIO_BIT(pin));
    if (falling) mmio_write32(GPIO_SET_FAL_TRIG01_OFS, GPIO_BIT(pin));
    return TMS320C6452_GPIO_OK;
}

int tms320c6452_gpio_disable_edge(uint32_t pin, bool rising, bool falling)
{
    if (!valid_pin(pin)) return TMS320C6452_GPIO_ERR_INVAL;
    if (rising)  mmio_write32(GPIO_CLR_RIS_TRIG01_OFS, GPIO_BIT(pin));
    if (falling) mmio_write32(GPIO_CLR_FAL_TRIG01_OFS, GPIO_BIT(pin));
    return TMS320C6452_GPIO_OK;
}

int tms320c6452_gpio_bank_int_enable(uint32_t bank, bool enable)
{
    if (!valid_bank(bank)) return TMS320C6452_GPIO_ERR_INVAL;
    uint32_t mask = (bank == 0u) ? GPIO_BINTEN_EN0 : GPIO_BINTEN_EN1;
    if (enable) mmio_setbits32(GPIO_BINTEN_OFS, mask);
    else        mmio_clrbits32(GPIO_BINTEN_OFS, mask);
    return TMS320C6452_GPIO_OK;
}

uint32_t tms320c6452_gpio_int_status(void)
{
    return mmio_read32(GPIO_INTSTAT01_OFS);
}

void tms320c6452_gpio_isr(void)
{
    /* ISR flow: read and clear INTSTAT01 (W1C). */
    uint32_t status = mmio_read32(GPIO_INTSTAT01_OFS);
    if (status) {
        mmio_write32(GPIO_INTSTAT01_OFS, status); /* W1C */
    }
}
