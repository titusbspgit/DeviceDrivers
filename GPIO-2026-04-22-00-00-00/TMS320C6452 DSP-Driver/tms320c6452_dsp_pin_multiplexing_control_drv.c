/*
 * Feature Driver: Pin multiplexing control
 *
 * Pinmux registers reside outside GPIO and are device-specific.
 * This driver stubs out configuration with UNKNOWN markers.
 */
#include <stddef.h>
#include "tms320c6452_dsp_pin_multiplexing_control.h"

/* Validate that a GPIO pin could be muxed; without device data, return unsupported. */
gpio_drv_status_t gpio_pinmux_validate(uint32_t gpio_pin)
{
    (void)gpio_pin; /* not used without device-specific tables */
    return GPIO_DRV_ERR_UNSUPPORTED; /* UNKNOWN pinmux tables */
}

/* Attempt to configure pinmux for a GPIO pin. Without device tables, report unsupported. */
gpio_drv_status_t gpio_pinmux_configure(uint32_t gpio_pin, uint32_t mux_sel)
{
    (void)gpio_pin;
    (void)mux_sel;
    return GPIO_DRV_ERR_UNSUPPORTED; /* Requires device data manual */
}
