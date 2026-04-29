/* GPIO Basic Output single-feature driver (MISRA-like, deterministic) */
#include "tms320c6452_dsp_gpio_basic_output.h"

/* INTERNAL: Safe pin mask builder (assumes max 32 GPIOs unless UNKNOWN is clarified) */
static inline uint32_t gpio_pin_mask(uint32_t pin)
{
    if (pin >= 32u) { /* UNKNOWN width; conservative check */
        return 0u;
    }
    return (uint32_t)(1u << pin);
}

/* Helpers to get register addresses (use big-endian MMIO via core ops) */
static inline volatile uint32_t *reg_addr(uint32_t offset)
{
    return (volatile uint32_t *)(TMS320C6452_DSP_BASE + offset);
}

int tms320c6452_dsp_gpio_basic_output_init(void)
{
    /* UNKNOWN: No specific initialization sequence known. */
    tms320c6452_dsp_core_ops.dmb();
    return GPIO_BO_OK;
}

int tms320c6452_dsp_gpio_basic_output_config_pin(uint32_t pin)
{
    const uint32_t mask = gpio_pin_mask(pin);
    volatile uint32_t * const dir = reg_addr(GPIO_DIR_OFFSET);

    if (mask == 0u) {
        return GPIO_BO_EINVAL;
    }

    /* Configure as output: conventionally set bit=1 for output (UNKNOWN) */
    if (GPIO_DIR_OFFSET == 0u) {
        /* UNKNOWN: cannot access register map without valid offset */
        return GPIO_BO_EUNSUPPORTED;
    }

    uint32_t v = tms320c6452_dsp_core_ops.read32(dir);
    v |= mask; /* UNKNOWN policy; adjust when map known */
    tms320c6452_dsp_core_ops.write32(dir, v);
    tms320c6452_dsp_core_ops.dsb();
    return GPIO_BO_OK;
}

int tms320c6452_dsp_gpio_basic_output_write(uint32_t pin, uint8_t level)
{
    const uint32_t mask = gpio_pin_mask(pin);
    if (mask == 0u) {
        return GPIO_BO_EINVAL;
    }

    /* Prefer W1S/W1C if available; else write OUT reg */
    if ((level != 0u) && (GPIO_OUT_SET_OFFSET != 0u)) {
        tms320c6452_dsp_core_ops.write32(reg_addr(GPIO_OUT_SET_OFFSET), mask);
        return GPIO_BO_OK;
    } else if ((level == 0u) && (GPIO_OUT_CLR_OFFSET != 0u)) {
        tms320c6452_dsp_core_ops.write32(reg_addr(GPIO_OUT_CLR_OFFSET), mask);
        return GPIO_BO_OK;
    } else {
        if (GPIO_OUT_OFFSET == 0u) {
            return GPIO_BO_EUNSUPPORTED; /* UNKNOWN map */
        }
        volatile uint32_t * const out = reg_addr(GPIO_OUT_OFFSET);
        uint32_t v = tms320c6452_dsp_core_ops.read32(out);
        if (level != 0u) {
            v |= mask;
        } else {
            v &= (uint32_t)(~mask);
        }
        tms320c6452_dsp_core_ops.write32(out, v);
        return GPIO_BO_OK;
    }
}

int tms320c6452_dsp_gpio_basic_output_toggle(uint32_t pin)
{
    const uint32_t mask = gpio_pin_mask(pin);
    if (mask == 0u) {
        return GPIO_BO_EINVAL;
    }

    if (GPIO_OUT_OFFSET == 0u) {
        return GPIO_BO_EUNSUPPORTED; /* UNKNOWN map */
    }
    volatile uint32_t * const out = reg_addr(GPIO_OUT_OFFSET);
    uint32_t v = tms320c6452_dsp_core_ops.read32(out);
    v ^= mask;
    tms320c6452_dsp_core_ops.write32(out, v);
    return GPIO_BO_OK;
}

int tms320c6452_dsp_gpio_basic_output_read(uint32_t pin, uint8_t *level)
{
    const uint32_t mask = gpio_pin_mask(pin);
    if ((mask == 0u) || (level == (uint8_t *)0)) {
        return GPIO_BO_EINVAL;
    }

    if (GPIO_IN_OFFSET == 0u) {
        return GPIO_BO_EUNSUPPORTED; /* UNKNOWN map */
    }

    volatile uint32_t * const inr = reg_addr(GPIO_IN_OFFSET);
    const uint32_t v = tms320c6452_dsp_core_ops.read32(inr);
    *level = (uint8_t)((v & mask) != 0u ? 1u : 0u);
    return GPIO_BO_OK;
}

void tms320c6452_dsp_gpio_basic_output_deinit(void)
{
    /* UNKNOWN: No specific deinit sequence known. */
    tms320c6452_dsp_core_ops.dmb();
}
