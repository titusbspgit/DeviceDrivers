/*
 * TMS320C6452 GPIO Driver - Source
 * Environment: Bare-metal, big-endian, GCC
 * Controller base: 0xA1008000
 */
#include "../include/gpio_tms320c6452.h"

#ifndef STATIC
#define STATIC static
#endif

/* Internal GPIO base pointer */
STATIC volatile uint32_t * gp_gpio_base = (volatile uint32_t *)(void *)TMS320C6452_GPIO_BASE_ADDR;
STATIC bool g_initialized = false;

/* MMIO helpers */
STATIC inline void mmio_write32(uintptr_t offset, uint32_t value)
{
    volatile uint32_t *addr = (volatile uint32_t *)(void *)((uintptr_t)gp_gpio_base + offset);
    *addr = value;
}

STATIC inline uint32_t mmio_read32(uintptr_t offset)
{
    volatile const uint32_t *addr = (volatile const uint32_t *)(const void *)((uintptr_t)gp_gpio_base + offset);
    return *addr;
}

/* Parameter checks */
STATIC inline bool pin_valid(uint32_t pin)
{
    return (pin < GPIO_PIN_MAX);
}

STATIC inline bool bank_valid(uint32_t bank)
{
    return (bank < GPIO_BANKS);
}

/* Weak HAL defaults */
__attribute__((weak)) bool gpio_hal_enable_clock(void)
{
    return true;
}

__attribute__((weak)) bool gpio_hal_configure_pinmux(uint32_t pin)
{
    (void)pin;
    return true;
}

/* Public APIs */

gpio_status_t gpio_init(uintptr_t base)
{
    if (base != (uintptr_t)0U)
    {
        gp_gpio_base = (volatile uint32_t *)(void *)base;
    }

    if (!gpio_hal_enable_clock())
    {
        return GPIO_STATUS_HW_MISMATCH;
    }

    (void)mmio_read32(GPIO_REG_PID_OFFSET);
    g_initialized = true;
    return GPIO_STATUS_OK;
}

gpio_status_t gpio_get_pid(uint32_t *pid_out)
{
    if (pid_out == NULL)
    {
        return GPIO_STATUS_INVALID_ARG;
    }
    if (!g_initialized)
    {
        return GPIO_STATUS_NOT_INITIALIZED;
    }
    *pid_out = mmio_read32(GPIO_REG_PID_OFFSET);
    return GPIO_STATUS_OK;
}

gpio_status_t gpio_set_direction(uint32_t pin, gpio_dir_t dir)
{
    if (!g_initialized)
    {
        return GPIO_STATUS_NOT_INITIALIZED;
    }
    if (!pin_valid(pin) || ((dir != GPIO_DIR_INPUT) && (dir != GPIO_DIR_OUTPUT)))
    {
        return GPIO_STATUS_INVALID_ARG;
    }

    (void)gpio_hal_configure_pinmux(pin);

    uint32_t dir_reg = mmio_read32(GPIO_REG_DIR01_OFFSET);
    uint32_t mask = GPIO_PIN_MASK(pin);
    if (dir == GPIO_DIR_INPUT)
    {
        dir_reg |= mask;  /* 1 = input */
    }
    else
    {
        dir_reg &= ~mask; /* 0 = output */
    }
    mmio_write32(GPIO_REG_DIR01_OFFSET, dir_reg);
    return GPIO_STATUS_OK;
}

gpio_status_t gpio_get_direction(uint32_t pin, gpio_dir_t *dir_out)
{
    if ((dir_out == NULL) || !g_initialized)
    {
        return (dir_out == NULL) ? GPIO_STATUS_INVALID_ARG : GPIO_STATUS_NOT_INITIALIZED;
    }
    if (!pin_valid(pin))
    {
        return GPIO_STATUS_INVALID_ARG;
    }
    uint32_t dir_reg = mmio_read32(GPIO_REG_DIR01_OFFSET);
    *dir_out = ((dir_reg & GPIO_PIN_MASK(pin)) != 0U) ? GPIO_DIR_INPUT : GPIO_DIR_OUTPUT;
    return GPIO_STATUS_OK;
}

gpio_status_t gpio_write_pin(uint32_t pin, bool value)
{
    if (!g_initialized)
    {
        return GPIO_STATUS_NOT_INITIALIZED;
    }
    if (!pin_valid(pin))
    {
        return GPIO_STATUS_INVALID_ARG;
    }
    uint32_t mask = GPIO_PIN_MASK(pin);
    if (value)
    {
        mmio_write32(GPIO_REG_SET_DATA01_OFFSET, mask);
    }
    else
    {
        mmio_write32(GPIO_REG_CLR_DATA01_OFFSET, mask);
    }
    return GPIO_STATUS_OK;
}

gpio_status_t gpio_read_pin(uint32_t pin, bool *value_out)
{
    if ((value_out == NULL) || !g_initialized)
    {
        return (value_out == NULL) ? GPIO_STATUS_INVALID_ARG : GPIO_STATUS_NOT_INITIALIZED;
    }
    if (!pin_valid(pin))
    {
        return GPIO_STATUS_INVALID_ARG;
    }
    uint32_t in = mmio_read32(GPIO_REG_IN_DATA01_OFFSET);
    *value_out = ((in & GPIO_PIN_MASK(pin)) != 0U) ? true : false;
    return GPIO_STATUS_OK;
}

gpio_status_t gpio_write_mask(uint32_t set_mask, uint32_t clear_mask)
{
    if (!g_initialized)
    {
        return GPIO_STATUS_NOT_INITIALIZED;
    }
    if (clear_mask != 0U)
    {
        mmio_write32(GPIO_REG_CLR_DATA01_OFFSET, clear_mask);
    }
    if (set_mask != 0U)
    {
        mmio_write32(GPIO_REG_SET_DATA01_OFFSET, set_mask);
    }
    return GPIO_STATUS_OK;
}

gpio_status_t gpio_irq_configure_pin(uint32_t pin, gpio_edge_t edge_sel)
{
    if (!g_initialized)
    {
        return GPIO_STATUS_NOT_INITIALIZED;
    }
    if (!pin_valid(pin))
    {
        return GPIO_STATUS_INVALID_ARG;
    }
    uint32_t mask = GPIO_PIN_MASK(pin);

    mmio_write32(GPIO_REG_CLR_RIS_TRIG01_OFFSET, mask);
    mmio_write32(GPIO_REG_CLR_FAL_TRIG01_OFFSET, mask);

    switch (edge_sel)
    {
        case GPIO_EDGE_NONE:
            break;
        case GPIO_EDGE_RISING:
            mmio_write32(GPIO_REG_SET_RIS_TRIG01_OFFSET, mask);
            break;
        case GPIO_EDGE_FALLING:
            mmio_write32(GPIO_REG_SET_FAL_TRIG01_OFFSET, mask);
            break;
        case GPIO_EDGE_BOTH:
            mmio_write32(GPIO_REG_SET_RIS_TRIG01_OFFSET, mask);
            mmio_write32(GPIO_REG_SET_FAL_TRIG01_OFFSET, mask);
            break;
        default:
            return GPIO_STATUS_INVALID_ARG;
    }
    return GPIO_STATUS_OK;
}

gpio_status_t gpio_irq_bank_enable(uint32_t bank, bool enable)
{
    if (!g_initialized)
    {
        return GPIO_STATUS_NOT_INITIALIZED;
    }
    if (!bank_valid(bank))
    {
        return GPIO_STATUS_INVALID_ARG;
    }
    uint32_t binten = mmio_read32(GPIO_REG_BINTEN_OFFSET);
    uint32_t bit = (bank == 0U) ? GPIO_BINTEN_EN0 : GPIO_BINTEN_EN1;
    if (enable)
    {
        binten |= bit;
    }
    else
    {
        binten &= ~bit;
    }
    mmio_write32(GPIO_REG_BINTEN_OFFSET, (binten & (GPIO_BINTEN_EN0 | GPIO_BINTEN_EN1)));
    return GPIO_STATUS_OK;
}

gpio_status_t gpio_irq_get_status(uint32_t *pending_mask_out)
{
    if ((pending_mask_out == NULL) || !g_initialized)
    {
        return (pending_mask_out == NULL) ? GPIO_STATUS_INVALID_ARG : GPIO_STATUS_NOT_INITIALIZED;
    }
    *pending_mask_out = mmio_read32(GPIO_REG_INTSTAT01_OFFSET);
    return GPIO_STATUS_OK;
}

gpio_status_t gpio_irq_clear_status(uint32_t clear_mask)
{
    if (!g_initialized)
    {
        return GPIO_STATUS_NOT_INITIALIZED;
    }
    if (clear_mask != 0U)
    {
        mmio_write32(GPIO_REG_INTSTAT01_OFFSET, clear_mask);
    }
    return GPIO_STATUS_OK;
}

gpio_status_t gpio_isr_service(uint32_t bank, uint32_t *cleared_mask_out)
{
    if ((cleared_mask_out == NULL) || !g_initialized)
    {
        return (cleared_mask_out == NULL) ? GPIO_STATUS_INVALID_ARG : GPIO_STATUS_NOT_INITIALIZED;
    }
    if (!bank_valid(bank))
    {
        return GPIO_STATUS_INVALID_ARG;
    }

    uint32_t pending = mmio_read32(GPIO_REG_INTSTAT01_OFFSET);
    uint32_t bank_mask = (bank == 0U) ? 0x0000FFFFu : 0xFFFF0000u;
    uint32_t to_clear = (pending & bank_mask);

    if (to_clear != 0U)
    {
        mmio_write32(GPIO_REG_INTSTAT01_OFFSET, to_clear);
    }

    *cleared_mask_out = to_clear;
    return GPIO_STATUS_OK;
}
