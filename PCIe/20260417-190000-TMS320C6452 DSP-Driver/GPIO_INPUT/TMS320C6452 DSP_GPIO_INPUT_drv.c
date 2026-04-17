/*
 * TMS320C6452 DSP GPIO INPUT feature - Driver Source
 * Target: ARM (big-endian), Baremetal, gcc
 *
 * Provides: initialization, configure-as-input (with optional pull via HAL),
 * and read APIs. Uses deterministic memory-mapped register access with
 * volatile semantics. No dynamic allocation. No undefined behavior.
 */

#include "TMS320C6452 DSP_GPIO_INPUT.h"

/* Local helpers */
#define MMIO32(addr)    (*(volatile uint32_t *)(addr))

static inline uint32_t gpio_reg_read(uint32_t offset)
{
    return MMIO32(GPIO_BASE_ADDR + offset);
}

static inline void gpio_reg_write(uint32_t offset, uint32_t value)
{
    MMIO32(GPIO_BASE_ADDR + offset) = value;
}

/* Weak HAL hooks (override in platform code as needed) */
__attribute__((weak)) int hal_gpio_enable_module_clock(void)
{
    /* Platform should enable PSC for GPIO. Default: success. */
    return 0;
}

__attribute__((weak)) int hal_pinmux_select_gpio(uint32_t pin)
{
    (void)pin;
    /* Platform should select GPIO function for the pad. Default: success. */
    return 0;
}

__attribute__((weak)) int hal_pad_config_pull(uint32_t pin, gpio_pull_t pull)
{
    (void)pin;
    (void)pull;
    /* Platform should configure pad pull-up/down if supported. Default: success. */
    return 0;
}

void gpio_input_init(void)
{
    (void)hal_gpio_enable_module_clock();
}

static int validate_pin(uint32_t pin)
{
    return (pin < 32UL) ? 1 : 0;
}

gpio_status_t gpio_input_configure(uint32_t pin, gpio_pull_t pull)
{
    if (validate_pin(pin) == 0)
    {
        return GPIO_STATUS_EINVAL;
    }

    /* Ensure GPIO clock and pin function are set */
    if (hal_pinmux_select_gpio(pin) != 0)
    {
        return GPIO_STATUS_EHW;
    }

    if (hal_pad_config_pull(pin, pull) != 0)
    {
        return GPIO_STATUS_EHW;
    }

    /* Set direction bit to input (1) for the chosen pin */
    uint32_t dir = gpio_reg_read(GPIO_DIR01_OFFSET);
    dir |= GPIO_BIT(pin);
    gpio_reg_write(GPIO_DIR01_OFFSET, dir);

    return GPIO_STATUS_OK;
}

gpio_status_t gpio_input_read(uint32_t pin, uint32_t *level)
{
    if ((validate_pin(pin) == 0) || (level == (uint32_t *)0))
    {
        return GPIO_STATUS_EINVAL;
    }

    uint32_t val = gpio_reg_read(GPIO_IN_DATA01_OFFSET);
    *level = ((val & GPIO_BIT(pin)) != 0U) ? 1UL : 0UL;
    return GPIO_STATUS_OK;
}

uint32_t gpio_get_dir_reg(void)
{
    return gpio_reg_read(GPIO_DIR01_OFFSET);
}
