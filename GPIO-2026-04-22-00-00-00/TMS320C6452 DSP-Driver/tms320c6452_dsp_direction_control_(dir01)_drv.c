/*
 * Feature Driver: Direction control (DIR01)
 *
 * Without a documented DIR01 offset, hardware access is disabled.
 * APIs return GPIO_DRV_ERR_UNKNOWN_REG to avoid unsafe MMIO.
 */
#include <stddef.h>
#include "tms320c6452_dsp_direction_control_(dir01).h"

/* Optional software shadow for testing when HW address is unknown */
static uint32_t s_dir_shadow = 0xFFFFFFFFu; /* reset default: inputs (1) */

static gpio_drv_status_t dir_unsupported(void)
{
    return GPIO_DRV_ERR_UNKNOWN_REG;
}

gpio_drv_status_t gpio_dir_init(void)
{
    /* No hardware writes without known offset. Maintain shadow only. */
    s_dir_shadow = 0xFFFFFFFFu; /* inputs */
    return GPIO_DRV_OK;
}

gpio_drv_status_t gpio_set_pin_output(uint32_t pin)
{
    if (!GPIO_IS_VALID_PIN(pin)) { return GPIO_DRV_ERR_INVALID_PARAM; }
#if defined(GPIO_DIR01_OFFSET_UNKNOWN)
    /* Update shadow, report unknown register for HW */
    s_dir_shadow &= (uint32_t)(~GPIO_PIN_MASK(pin));
    return dir_unsupported();
#else
    /* Not reached in this configuration */
    return GPIO_DRV_ERR_UNSUPPORTED;
#endif
}

gpio_drv_status_t gpio_set_pin_input(uint32_t pin)
{
    if (!GPIO_IS_VALID_PIN(pin)) { return GPIO_DRV_ERR_INVALID_PARAM; }
#if defined(GPIO_DIR01_OFFSET_UNKNOWN)
    s_dir_shadow |= GPIO_PIN_MASK(pin);
    return dir_unsupported();
#else
    return GPIO_DRV_ERR_UNSUPPORTED;
#endif
}

gpio_drv_status_t gpio_get_pin_is_input(uint32_t pin, uint32_t *is_input)
{
    if (!GPIO_IS_VALID_PIN(pin) || (is_input == NULL)) { return GPIO_DRV_ERR_INVALID_PARAM; }
#if defined(GPIO_DIR01_OFFSET_UNKNOWN)
    *is_input = ( (s_dir_shadow & GPIO_PIN_MASK(pin)) != 0u ) ? 1u : 0u;
    return dir_unsupported();
#else
    return GPIO_DRV_ERR_UNSUPPORTED;
#endif
}
