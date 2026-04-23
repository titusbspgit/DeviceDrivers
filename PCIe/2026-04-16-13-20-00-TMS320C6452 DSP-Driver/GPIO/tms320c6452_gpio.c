/*
 * TMS320C6452 GPIO Driver - Source
 * RCI-ag-DrvGen Agent
 *
 * Implements register-level driver for GPIO peripheral per SPRUF95.
 * Deterministic register access; no dynamic allocation; portable C.
 */
#include "tms320c6452_gpio.h"

/* Internal helpers */
static inline bool tms320c6452_gpio_pin_valid(uint32_t pin)
{
    return (pin < 32u);
}

static inline uint32_t tms320c6452_gpio_bit(uint32_t pin)
{
    return TMS320C6452_GPIO_PIN_MASK(pin);
}

/* Initialize driver context and optionally verify PID */
tms320c6452_gpio_status_t tms320c6452_gpio_init(tms320c6452_gpio_t * ctx, uintptr_t base, bool verify_pid)
{
    if (ctx == NULL)
    {
        return TMS320C6452_GPIO_STATUS_EINVAL;
    }

    ctx->base = base;

    if (verify_pid)
    {
        const uint32_t pid = tms320c6452_gpio_reg_read(ctx, TMS320C6452_GPIO_PID_OFFSET);
        const uint32_t function = (pid & TMS320C6452_GPIO_PID_FUNCTION_MASK) >> TMS320C6452_GPIO_PID_FUNCTION_SHIFT;
        if (function != TMS320C6452_GPIO_PID_FUNCTION_GPIO)
        {
            return TMS320C6452_GPIO_STATUS_EHWID;
        }
    }

    return TMS320C6452_GPIO_STATUS_OK;
}

uint32_t tms320c6452_gpio_get_pid(const tms320c6452_gpio_t * ctx)
{
    if (ctx == NULL)
    {
        return 0u;
    }
    return tms320c6452_gpio_reg_read((const volatile tms320c6452_gpio_t *)ctx, TMS320C6452_GPIO_PID_OFFSET);
}

/* Direction control */
tms320c6452_gpio_status_t tms320c6452_gpio_set_pin_direction(const tms320c6452_gpio_t * ctx, uint32_t pin, bool input)
{
    if ((ctx == NULL) || (!tms320c6452_gpio_pin_valid(pin)))
    {
        return TMS320C6452_GPIO_STATUS_EINVAL;
    }

    const uint32_t mask = tms320c6452_gpio_bit(pin);
    uint32_t dir = tms320c6452_gpio_reg_read(ctx, TMS320C6452_GPIO_DIR01_OFFSET);
    if (input)
    {
        dir |= mask;  /* set to input */
    }
    else
    {
        dir &= ~mask; /* clear to output */
    }
    tms320c6452_gpio_reg_write(ctx, TMS320C6452_GPIO_DIR01_OFFSET, dir);
    return TMS320C6452_GPIO_STATUS_OK;
}

/* Batch set inputs: sets selected bits in DIR to 1 */
tms320c6452_gpio_status_t tms320c6452_gpio_set_inputs_mask(const tms320c6452_gpio_t * ctx, uint32_t mask)
{
    if (ctx == NULL)
    {
        return TMS320C6452_GPIO_STATUS_EINVAL;
    }
    uint32_t dir = tms320c6452_gpio_reg_read(ctx, TMS320C6452_GPIO_DIR01_OFFSET);
    dir |= mask;
    tms320c6452_gpio_reg_write(ctx, TMS320C6452_GPIO_DIR01_OFFSET, dir);
    return TMS320C6452_GPIO_STATUS_OK;
}

/* Batch set outputs: clears selected bits in DIR to 0 */
tms320c6452_gpio_status_t tms320c6452_gpio_set_outputs_mask(const tms320c6452_gpio_t * ctx, uint32_t mask)
{
    if (ctx == NULL)
    {
        return TMS320C6452_GPIO_STATUS_EINVAL;
    }
    uint32_t dir = tms320c6452_gpio_reg_read(ctx, TMS320C6452_GPIO_DIR01_OFFSET);
    dir &= ~mask;
    tms320c6452_gpio_reg_write(ctx, TMS320C6452_GPIO_DIR01_OFFSET, dir);
    return TMS320C6452_GPIO_STATUS_OK;
}

/* Output control */
tms320c6452_gpio_status_t tms320c6452_gpio_write_pin(const tms320c6452_gpio_t * ctx, uint32_t pin, bool high)
{
    if ((ctx == NULL) || (!tms320c6452_gpio_pin_valid(pin)))
    {
        return TMS320C6452_GPIO_STATUS_EINVAL;
    }
    const uint32_t mask = tms320c6452_gpio_bit(pin);
    if (high)
    {
        tms320c6452_gpio_reg_write(ctx, TMS320C6452_GPIO_SET_DATA01_OFFSET, mask);
    }
    else
    {
        tms320c6452_gpio_reg_write(ctx, TMS320C6452_GPIO_CLR_DATA01_OFFSET, mask);
    }
    return TMS320C6452_GPIO_STATUS_OK;
}

tms320c6452_gpio_status_t tms320c6452_gpio_set_pins(const tms320c6452_gpio_t * ctx, uint32_t mask)
{
    if (ctx == NULL)
    {
        return TMS320C6452_GPIO_STATUS_EINVAL;
    }
    tms320c6452_gpio_reg_write(ctx, TMS320C6452_GPIO_SET_DATA01_OFFSET, mask);
    return TMS320C6452_GPIO_STATUS_OK;
}

tms320c6452_gpio_status_t tms320c6452_gpio_clear_pins(const tms320c6452_gpio_t * ctx, uint32_t mask)
{
    if (ctx == NULL)
    {
        return TMS320C6452_GPIO_STATUS_EINVAL;
    }
    tms320c6452_gpio_reg_write(ctx, TMS320C6452_GPIO_CLR_DATA01_OFFSET, mask);
    return TMS320C6452_GPIO_STATUS_OK;
}

tms320c6452_gpio_status_t tms320c6452_gpio_toggle_pin(const tms320c6452_gpio_t * ctx, uint32_t pin)
{
    if ((ctx == NULL) || (!tms320c6452_gpio_pin_valid(pin)))
    {
        return TMS320C6452_GPIO_STATUS_EINVAL;
    }
    const uint32_t mask = tms320c6452_gpio_bit(pin);
    const uint32_t out = tms320c6452_gpio_reg_read(ctx, TMS320C6452_GPIO_OUT_DATA01_OFFSET);
    if ((out & mask) != 0u)
    {
        tms320c6452_gpio_reg_write(ctx, TMS320C6452_GPIO_CLR_DATA01_OFFSET, mask);
    }
    else
    {
        tms320c6452_gpio_reg_write(ctx, TMS320C6452_GPIO_SET_DATA01_OFFSET, mask);
    }
    return TMS320C6452_GPIO_STATUS_OK;
}

/* Reads */
tms320c6452_gpio_status_t tms320c6452_gpio_read_pin(const tms320c6452_gpio_t * ctx, uint32_t pin, bool * level)
{
    if ((ctx == NULL) || (level == NULL) || (!tms320c6452_gpio_pin_valid(pin)))
    {
        return TMS320C6452_GPIO_STATUS_EINVAL;
    }
    const uint32_t in = tms320c6452_gpio_reg_read(ctx, TMS320C6452_GPIO_IN_DATA01_OFFSET);
    *level = ((in & tms320c6452_gpio_bit(pin)) != 0u);
    return TMS320C6452_GPIO_STATUS_OK;
}

uint32_t tms320c6452_gpio_read_inputs(const tms320c6452_gpio_t * ctx)
{
    if (ctx == NULL)
    {
        return 0u;
    }
    return tms320c6452_gpio_reg_read(ctx, TMS320C6452_GPIO_IN_DATA01_OFFSET);
}

uint32_t tms320c6452_gpio_read_outputs_latch(const tms320c6452_gpio_t * ctx)
{
    if (ctx == NULL)
    {
        return 0u;
    }
    return tms320c6452_gpio_reg_read(ctx, TMS320C6452_GPIO_OUT_DATA01_OFFSET);
}

/* Interrupt configuration */
tms320c6452_gpio_status_t tms320c6452_gpio_bank_enable(const tms320c6452_gpio_t * ctx, tms320c6452_gpio_bank_t bank, bool enable)
{
    if (ctx == NULL)
    {
        return TMS320C6452_GPIO_STATUS_EINVAL;
    }
    uint32_t binten = tms320c6452_gpio_reg_read(ctx, TMS320C6452_GPIO_BINTEN_OFFSET);
    const uint32_t bit = (bank == TMS320C6452_GPIO_BANK0) ? TMS320C6452_GPIO_BINTEN_EN0 : TMS320C6452_GPIO_BINTEN_EN1;
    if (enable)
    {
        binten |= bit;
    }
    else
    {
        binten &= ~bit;
    }
    tms320c6452_gpio_reg_write(ctx, TMS320C6452_GPIO_BINTEN_OFFSET, binten);
    return TMS320C6452_GPIO_STATUS_OK;
}

/* Configure edge detection for a pin using W1S/W1C semantics */
tms320c6452_gpio_status_t tms320c6452_gpio_config_edge(const tms320c6452_gpio_t * ctx, uint32_t pin, bool enable_rising, bool enable_falling)
{
    if ((ctx == NULL) || (!tms320c6452_gpio_pin_valid(pin)))
    {
        return TMS320C6452_GPIO_STATUS_EINVAL;
    }
    const uint32_t bit = tms320c6452_gpio_bit(pin);
    if (enable_rising)
    {
        tms320c6452_gpio_reg_write(ctx, TMS320C6452_GPIO_SET_RIS_TRIG01_OFFSET, bit);
    }
    else
    {
        tms320c6452_gpio_reg_write(ctx, TMS320C6452_GPIO_CLR_RIS_TRIG01_OFFSET, bit);
    }

    if (enable_falling)
    {
        tms320c6452_gpio_reg_write(ctx, TMS320C6452_GPIO_SET_FAL_TRIG01_OFFSET, bit);
    }
    else
    {
        tms320c6452_gpio_reg_write(ctx, TMS320C6452_GPIO_CLR_FAL_TRIG01_OFFSET, bit);
    }

    return TMS320C6452_GPIO_STATUS_OK;
}

uint32_t tms320c6452_gpio_get_pending(const tms320c6452_gpio_t * ctx)
{
    if (ctx == NULL)
    {
        return 0u;
    }
    return tms320c6452_gpio_reg_read(ctx, TMS320C6452_GPIO_INTSTAT01_OFFSET);
}

void tms320c6452_gpio_clear_pending(const tms320c6452_gpio_t * ctx, uint32_t mask)
{
    if (ctx == NULL)
    {
        return;
    }
    /* INTSTAT is W1C: write ones to clear */
    tms320c6452_gpio_reg_write(ctx, TMS320C6452_GPIO_INTSTAT01_OFFSET, mask);
}
