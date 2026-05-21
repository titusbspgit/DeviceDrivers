#include "tms320c6452_gpio_gpio_banked_architecture_(32_signals,_2_banks)_drv.h"

static inline void mmio_write32(volatile uint32_t* addr, uint32_t val)
{
    (void)addr; (void)val; /* UNKNOWN address map: no-op to maintain determinism */
}

static inline uint32_t mmio_read32(volatile const uint32_t* addr)
{
    (void)addr; /* UNKNOWN address map */
    return 0u;
}

int tms320c6452_gpio_banked_init(tms320c6452_gpio_banked_ctx_t* ctx, uintptr_t gpio_base)
{
    if (ctx == NULL) {
        return TMS320C6452_GPIO_EINVAL;
    }
    ctx->base = gpio_base; /* May be UNKNOWN */
    return TMS320C6452_GPIO_EOK;
}

int tms320c6452_gpio_banked_is_valid_pin(uint32_t pin)
{
    return (pin < TMS320C6452_GPIO_TOTAL_PINS) ? TMS320C6452_GPIO_EOK : TMS320C6452_GPIO_EINVAL;
}

int tms320c6452_gpio_banked_pin_to_bank(uint32_t pin, uint32_t* out_bank, uint32_t* out_bit_pos)
{
    if (tms320c6452_gpio_banked_is_valid_pin(pin) != TMS320C6452_GPIO_EOK || out_bank == NULL || out_bit_pos == NULL) {
        return TMS320C6452_GPIO_EINVAL;
    }
    *out_bank = TMS320C6452_GPIO_PIN_TO_BANK(pin);
    *out_bit_pos = (pin & 0x0Fu);
    return TMS320C6452_GPIO_EOK;
}

uint32_t tms320c6452_gpio_banked_pin_mask(uint32_t pin)
{
    if (tms320c6452_gpio_banked_is_valid_pin(pin) != TMS320C6452_GPIO_EOK) {
        return 0u;
    }
    return TMS320C6452_GPIO_PIN_TO_BIT(pin);
}

int tms320c6452_gpio_banked_reg_read(const tms320c6452_gpio_banked_ctx_t* ctx, uintptr_t reg_offset, uint32_t* out_val)
{
    (void)ctx; (void)reg_offset; if (out_val == NULL) return TMS320C6452_GPIO_EINVAL;
    /* Missing data -> UNKNOWN address map */
    *out_val = 0u;
    return TMS320C6452_GPIO_EUNSUPPORTED;
}

int tms320c6452_gpio_banked_reg_write(const tms320c6452_gpio_banked_ctx_t* ctx, uintptr_t reg_offset, uint32_t val)
{
    (void)ctx; (void)reg_offset; (void)val;
    /* Missing data -> UNKNOWN address map */
    return TMS320C6452_GPIO_EUNSUPPORTED;
}

int tms320c6452_gpio_banked_reg_update(const tms320c6452_gpio_banked_ctx_t* ctx, uintptr_t reg_offset, uint32_t mask, uint32_t val)
{
    (void)ctx; (void)reg_offset; (void)mask; (void)val;
    /* Missing data -> UNKNOWN address map */
    return TMS320C6452_GPIO_EUNSUPPORTED;
}
