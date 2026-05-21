#include "tms320c6452_gpio_gpio_banked_architecture_(32_signals,_2_banks).h"

/* Deterministic memory-mapped I/O helpers */
static inline uint32_t mmio_read32(volatile uint32_t* addr)
{
    return *addr;
}

static inline void mmio_write32(volatile uint32_t* addr, uint32_t val)
{
    *addr = val;
    (void)*addr; /* read-back to order writes */
}

int tms320c6452_gpio_gpio_banked_architecture_init(tms320c6452_gpio_gpio_banked_architecture_ctx_t* ctx, uintptr_t gpio_base)
{
    if (ctx == NULL) {
        return TMS320C6452_GPIO_EFAULT;
    }
    ctx->base = gpio_base;
    return TMS320C6452_GPIO_EOK;
}

int tms320c6452_gpio_gpio_banked_architecture_pin_to_bank(uint32_t pin_index, uint32_t* out_bank_index, uint32_t* out_bit_index)
{
    if (out_bank_index == NULL || out_bit_index == NULL) {
        return TMS320C6452_GPIO_EFAULT;
    }
    if (pin_index >= TMS320C6452_GPIO_PIN_COUNT) {
        return TMS320C6452_GPIO_EINVAL;
    }
    *out_bank_index = (pin_index / TMS320C6452_GPIO_PINS_PER_BANK); /* 0 for [0..15], 1 for [16..31] */
    *out_bit_index  = (pin_index % TMS320C6452_GPIO_PINS_PER_BANK); /* 0..15 within bank */
    return TMS320C6452_GPIO_EOK;
}

int tms320c6452_gpio_gpio_banked_architecture_pin_to_mask(uint32_t pin_index, uint32_t* out_mask)
{
    if (out_mask == NULL) {
        return TMS320C6452_GPIO_EFAULT;
    }
    if (pin_index >= TMS320C6452_GPIO_PIN_COUNT) {
        return TMS320C6452_GPIO_EINVAL;
    }
    /* 32-bit wide control registers span both banks; mask maps directly to GP[31:0] bit position */
    *out_mask = (1u << pin_index);
    return TMS320C6452_GPIO_EOK;
}

int tms320c6452_gpio_gpio_banked_architecture_read32(const tms320c6452_gpio_gpio_banked_architecture_ctx_t* ctx, uint32_t reg_offset_bytes, uint32_t* out_val)
{
    if (ctx == NULL || out_val == NULL) {
        return TMS320C6452_GPIO_EFAULT;
    }
    volatile uint32_t* reg = (volatile uint32_t*)((uintptr_t)ctx->base + (uintptr_t)reg_offset_bytes);
    *out_val = mmio_read32(reg);
    return TMS320C6452_GPIO_EOK;
}

int tms320c6452_gpio_gpio_banked_architecture_write32(const tms320c6452_gpio_gpio_banked_architecture_ctx_t* ctx, uint32_t reg_offset_bytes, uint32_t val)
{
    if (ctx == NULL) {
        return TMS320C6452_GPIO_EFAULT;
    }
    volatile uint32_t* reg = (volatile uint32_t*)((uintptr_t)ctx->base + (uintptr_t)reg_offset_bytes);
    mmio_write32(reg, val);
    return TMS320C6452_GPIO_EOK;
}

int tms320c6452_gpio_gpio_banked_architecture_update_bits(const tms320c6452_gpio_gpio_banked_architecture_ctx_t* ctx, uint32_t reg_offset_bytes, uint32_t mask, uint32_t value_masked)
{
    if (ctx == NULL) {
        return TMS320C6452_GPIO_EFAULT;
    }
    volatile uint32_t* reg = (volatile uint32_t*)((uintptr_t)ctx->base + (uintptr_t)reg_offset_bytes);
    uint32_t cur = mmio_read32(reg);
    cur = (cur & ~mask) | (value_masked & mask);
    mmio_write32(reg, cur);
    return TMS320C6452_GPIO_EOK;
}
