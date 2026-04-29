#include "TMS320C6452 DSP_pin_mux_control.h"

/* Static state (MISRA-compliant, no dynamic allocation) */
static const struct gpio_core_ops* s_ops = (const struct gpio_core_ops*)0;
static uint32_t s_pinfunc_tbl[GPIO_PINMUX_MAX_PINS];
static bool s_mmio_enabled = false;

/* Local helpers */
static bool pin_valid(uint32_t pin)
{
    return (pin < GPIO_PINMUX_MAX_PINS);
}

static void table_init_defaults(void)
{
    uint32_t i;
    for (i = 0u; i < GPIO_PINMUX_MAX_PINS; ++i) {
        s_pinfunc_tbl[i] = 0u;
    }
}

static bool detect_mmio_capable(void)
{
    /* MMIO path requires known offsets AND valid ops.
       UNKNOWN markers force table mode (return false). */
    if (s_ops == (const struct gpio_core_ops*)0) {
        return false;
    }
    if ((PINMUX_BASE_OFFSET_UNKNOWN == 0xFFFFFFFFu) ||
        (PINMUX_FUNC_SEL_STRIDE_UNKNOWN == 0xFFFFFFFFu) ||
        (PINMUX_FUNC_SEL_BITS_UNKNOWN == 0xFFFFFFFFu)) {
        return false; /* UNKNOWN */
    }
    return true; /* Will be used if real offsets are provided in future */
}

int32_t gpio_pinmux_init(const struct gpio_core_ops* ops)
{
    s_ops = ops; /* ops may be NULL; table mode does not require ops */
    table_init_defaults();
    s_mmio_enabled = detect_mmio_capable();
    return GPIO_EOK;
}

int32_t gpio_pinmux_config(uint32_t pin, uint32_t function_sel)
{
    if (!pin_valid(pin)) {
        return GPIO_EINVAL;
    }

    if (s_mmio_enabled) {
        /* UNKNOWN: Device-specific MMIO programming sequence not available.
           Safe no-op returning EINVAL as per constraint. */
        (void)function_sel;
        return GPIO_EINVAL;
    } else {
        /* Table mode for testability; acts as configured state holder */
        s_pinfunc_tbl[pin] = function_sel;
        return GPIO_EOK;
    }
}

int32_t gpio_pinmux_get(uint32_t pin, uint32_t* function_sel_out)
{
    if (!pin_valid(pin)) {
        return GPIO_EINVAL;
    }
    if (function_sel_out == (uint32_t*)0) {
        return GPIO_EINVAL;
    }

    if (s_mmio_enabled) {
        /* UNKNOWN: Device-specific MMIO readback not available. */
        return GPIO_EINVAL;
    } else {
        *function_sel_out = s_pinfunc_tbl[pin];
        return GPIO_EOK;
    }
}
