/* TMS320C6452 DSP GPIO Driver - Source
 * RCI-ag-DrvGen Agent
 */
#include "../include/tms320c6452_gpio.h"

/* Internal helpers */
static inline bool tms_gpio_valid_pin(uint32_t pin) { return (pin < 32u); }
static inline bool tms_gpio_valid_bank(uint32_t bank) { return (bank < 2u); }

void tms_gpio_ctx_init(tms_gpio_ctx_t *ctx,
                       uintptr_t base,
                       tms_gpio_psc_ctrl_fn psc_enable,
                       tms_gpio_psc_ctrl_fn psc_disable,
                       tms_gpio_pinmux_validate_fn pinmux_validate)
{
    uint32_t i;
    if (ctx == (tms_gpio_ctx_t *)0) {
        return;
    }
    ctx->base = (base != 0u) ? base : (uintptr_t)TMS_GPIO_BASE_DEFAULT;
    ctx->psc_enable = psc_enable;
    ctx->psc_disable = psc_disable;
    ctx->pinmux_validate = pinmux_validate;
    for (i = 0u; i < 32u; ++i) {
        ctx->pin_cb[i] = (tms_gpio_pin_isr_cb_t)0;
        ctx->pin_cb_user[i] = (void *)0;
    }
    ctx->clock_enabled = false;
}

tms_gpio_status_t tms_gpio_enable_module(tms_gpio_ctx_t *ctx)
{
    if (ctx == (tms_gpio_ctx_t *)0) { return TMS_GPIO_EINVAL; }
    if (ctx->psc_enable != (tms_gpio_psc_ctrl_fn)0) {
        if (ctx->psc_enable() != 0) { return TMS_GPIO_ESTATE; }
    }
    ctx->clock_enabled = true;
    return TMS_GPIO_OK;
}

tms_gpio_status_t tms_gpio_disable_module(tms_gpio_ctx_t *ctx)
{
    if (ctx == (tms_gpio_ctx_t *)0) { return TMS_GPIO_EINVAL; }
    if (ctx->psc_disable != (tms_gpio_psc_ctrl_fn)0) {
        if (ctx->psc_disable() != 0) { return TMS_GPIO_ESTATE; }
    }
    ctx->clock_enabled = false;
    return TMS_GPIO_OK;
}

uint32_t tms_gpio_get_pid(const tms_gpio_ctx_t *ctx)
{
    if (ctx == (const tms_gpio_ctx_t *)0) { return 0u; }
    return tms_gpio_reg_read(ctx, TMS_GPIO_PID_OFS);
}

tms_gpio_status_t tms_gpio_set_direction(tms_gpio_ctx_t *ctx, uint32_t pin, bool input)
{
    uint32_t dir;
    uint32_t mask;
    if ((ctx == (tms_gpio_ctx_t *)0) || (!tms_gpio_valid_pin(pin))) { return TMS_GPIO_EINVAL; }
    mask = TMS_GPIO_PIN_MASK(pin);
    dir = tms_gpio_reg_read(ctx, TMS_GPIO_DIR01_OFS);
    if (input) { dir |= mask; }
    else { dir &= ~mask; }
    tms_gpio_reg_write(ctx, TMS_GPIO_DIR01_OFS, dir);
    return TMS_GPIO_OK;
}

tms_gpio_status_t tms_gpio_get_direction(const tms_gpio_ctx_t *ctx, uint32_t pin, bool *input_out)
{
    uint32_t dir;
    if ((ctx == (const tms_gpio_ctx_t *)0) || (!tms_gpio_valid_pin(pin)) || (input_out == (bool *)0)) { return TMS_GPIO_EINVAL; }
    dir = tms_gpio_reg_read(ctx, TMS_GPIO_DIR01_OFS);
    *input_out = ((dir & TMS_GPIO_PIN_MASK(pin)) != 0u);
    return TMS_GPIO_OK;
}

tms_gpio_status_t tms_gpio_write_pin(tms_gpio_ctx_t *ctx, uint32_t pin, bool level_high)
{
    uint32_t mask;
    if ((ctx == (tms_gpio_ctx_t *)0) || (!tms_gpio_valid_pin(pin))) { return TMS_GPIO_EINVAL; }
    mask = TMS_GPIO_PIN_MASK(pin);
    if (level_high) {
        tms_gpio_reg_write(ctx, TMS_GPIO_SET_DATA01_OFS, mask);
    } else {
        tms_gpio_reg_write(ctx, TMS_GPIO_CLR_DATA01_OFS, mask);
    }
    return TMS_GPIO_OK;
}

tms_gpio_status_t tms_gpio_set_mask(tms_gpio_ctx_t *ctx, uint32_t mask)
{
    if (ctx == (tms_gpio_ctx_t *)0) { return TMS_GPIO_EINVAL; }
    tms_gpio_reg_write(ctx, TMS_GPIO_SET_DATA01_OFS, mask);
    return TMS_GPIO_OK;
}

tms_gpio_status_t tms_gpio_clear_mask(tms_gpio_ctx_t *ctx, uint32_t mask)
{
    if (ctx == (tms_gpio_ctx_t *)0) { return TMS_GPIO_EINVAL; }
    tms_gpio_reg_write(ctx, TMS_GPIO_CLR_DATA01_OFS, mask);
    return TMS_GPIO_OK;
}

uint32_t tms_gpio_read_inputs(const tms_gpio_ctx_t *ctx)
{
    if (ctx == (const tms_gpio_ctx_t *)0) { return 0u; }
    return tms_gpio_reg_read(ctx, TMS_GPIO_IN_DATA01_OFS);
}

tms_gpio_status_t tms_gpio_config_edge(tms_gpio_ctx_t *ctx, uint32_t pin, bool rising_en, bool falling_en)
{
    uint32_t mask;
    if ((ctx == (tms_gpio_ctx_t *)0) || (!tms_gpio_valid_pin(pin))) { return TMS_GPIO_EINVAL; }
    mask = TMS_GPIO_PIN_MASK(pin);
    /* Program rising */
    if (rising_en) { tms_gpio_reg_write(ctx, TMS_GPIO_SET_RIS_TRIG01_OFS, mask); }
    else { tms_gpio_reg_write(ctx, TMS_GPIO_CLR_RIS_TRIG01_OFS, mask); }
    /* Program falling */
    if (falling_en) { tms_gpio_reg_write(ctx, TMS_GPIO_SET_FAL_TRIG01_OFS, mask); }
    else { tms_gpio_reg_write(ctx, TMS_GPIO_CLR_FAL_TRIG01_OFS, mask); }
    return TMS_GPIO_OK;
}

tms_gpio_status_t tms_gpio_enable_bank_irq(tms_gpio_ctx_t *ctx, uint32_t bank, bool enable)
{
    uint32_t val;
    if ((ctx == (tms_gpio_ctx_t *)0) || (!tms_gpio_valid_bank(bank))) { return TMS_GPIO_EINVAL; }
    val = tms_gpio_reg_read(ctx, TMS_GPIO_BINTEN_OFS);
    if (enable) {
        if (bank == 0u) { val |= TMS_GPIO_BINTEN_EN0_MASK; }
        else { val |= TMS_GPIO_BINTEN_EN1_MASK; }
    } else {
        if (bank == 0u) { val &= ~TMS_GPIO_BINTEN_EN0_MASK; }
        else { val &= ~TMS_GPIO_BINTEN_EN1_MASK; }
    }
    tms_gpio_reg_write(ctx, TMS_GPIO_BINTEN_OFS, val);
    return TMS_GPIO_OK;
}

uint32_t tms_gpio_get_irq_status(const tms_gpio_ctx_t *ctx)
{
    if (ctx == (const tms_gpio_ctx_t *)0) { return 0u; }
    return tms_gpio_reg_read(ctx, TMS_GPIO_INTSTAT01_OFS);
}

void tms_gpio_clear_irq_status(tms_gpio_ctx_t *ctx, uint32_t mask)
{
    if (ctx == (tms_gpio_ctx_t *)0) { return; }
    /* W1C: write only 1s to clear those bits */
    tms_gpio_reg_write(ctx, TMS_GPIO_INTSTAT01_OFS, mask);
}

tms_gpio_status_t tms_gpio_register_pin_isr(tms_gpio_ctx_t *ctx, uint32_t pin, tms_gpio_pin_isr_cb_t cb, void *user)
{
    if ((ctx == (tms_gpio_ctx_t *)0) || (!tms_gpio_valid_pin(pin))) { return TMS_GPIO_EINVAL; }
    ctx->pin_cb[pin] = cb;
    ctx->pin_cb_user[pin] = user;
    return TMS_GPIO_OK;
}

void tms_gpio_service_bank_isr(tms_gpio_ctx_t *ctx, uint32_t bank)
{
    uint32_t status;
    uint32_t mask;
    uint32_t shift;
    uint32_t bit;
    if ((ctx == (tms_gpio_ctx_t *)0) || (!tms_gpio_valid_bank(bank))) { return; }
    status = tms_gpio_get_irq_status(ctx);
    mask = (bank == 0u) ? TMS_GPIO_BANK0_MASK : TMS_GPIO_BANK1_MASK;
    shift = (bank == 0u) ? 0u : 16u;
    status &= mask;
    if (status != 0u) {
        /* Clear latched bits first to avoid losing edges */
        tms_gpio_clear_irq_status(ctx, status);
        for (bit = 0u; bit < 16u; ++bit) {
            if ((status & (1u << (bit + shift))) != 0u) {
                uint32_t pin = bit + shift;
                if (ctx->pin_cb[pin] != (tms_gpio_pin_isr_cb_t)0) {
                    ctx->pin_cb[pin](ctx->pin_cb_user[pin], pin);
                }
            }
        }
    }
}

tms_gpio_status_t tms_gpio_quiesce_interrupts(tms_gpio_ctx_t *ctx)
{
    if (ctx == (tms_gpio_ctx_t *)0) { return TMS_GPIO_EINVAL; }
    /* Disable both banks */
    tms_gpio_reg_write(ctx, TMS_GPIO_BINTEN_OFS, 0u);
    /* Clear any pending */
    tms_gpio_clear_irq_status(ctx, 0xFFFFFFFFu);
    return TMS_GPIO_OK;
}

tms_gpio_status_t tms_gpio_enter_low_power(tms_gpio_ctx_t *ctx)
{
    /* Quiesce interrupts then disable clock via PSC hook */
    tms_gpio_status_t st;
    if (ctx == (tms_gpio_ctx_t *)0) { return TMS_GPIO_EINVAL; }
    st = tms_gpio_quiesce_interrupts(ctx);
    if (st != TMS_GPIO_OK) { return st; }
    return tms_gpio_disable_module(ctx);
}

tms_gpio_status_t tms_gpio_exit_low_power(tms_gpio_ctx_t *ctx)
{
    if (ctx == (tms_gpio_ctx_t *)0) { return TMS_GPIO_EINVAL; }
    return tms_gpio_enable_module(ctx);
}

/* Feature-tagged helper implementations */
uint32_t tms_gpio_feature_banked_arch_get_bank_mask(uint32_t bank)
{
    return (bank == 0u) ? TMS_GPIO_BANK0_MASK : TMS_GPIO_BANK1_MASK;
}

tms_gpio_status_t tms_gpio_feature_pinmux_validate(tms_gpio_ctx_t *ctx)
{
    if (ctx == (tms_gpio_ctx_t *)0) { return TMS_GPIO_EINVAL; }
    if (ctx->pinmux_validate != (tms_gpio_pinmux_validate_fn)0) {
        return (ctx->pinmux_validate() == 0) ? TMS_GPIO_OK : TMS_GPIO_ESTATE;
    }
    /* If no validator is provided, assume already configured. */
    return TMS_GPIO_OK;
}

tms_gpio_status_t tms_gpio_feature_endianness_check(void)
{
    /* Per spec, GPIO IP is endian-agnostic for 32-bit word accesses. */
    return TMS_GPIO_OK;
}

void tms_gpio_feature_reset_reinit_sequence(tms_gpio_ctx_t *ctx)
{
    /* If PSC reset was performed externally, re-run init sequence as needed. */
    (void)tms_gpio_feature_pinmux_validate(ctx);
    (void)tms_gpio_enable_module(ctx);
    /* Default: inputs per reset; clear interrupts */
    tms_gpio_clear_irq_status(ctx, 0xFFFFFFFFu);
}

tms_gpio_status_t tms_gpio_feature_full_init(tms_gpio_ctx_t *ctx)
{
    tms_gpio_status_t st;
    if (ctx == (tms_gpio_ctx_t *)0) { return TMS_GPIO_EINVAL; }
    st = tms_gpio_feature_pinmux_validate(ctx);
    if (st != TMS_GPIO_OK) { return st; }
    st = tms_gpio_enable_module(ctx);
    if (st != TMS_GPIO_OK) { return st; }
    /* After clock enable, clear any stale interrupts */
    tms_gpio_clear_irq_status(ctx, 0xFFFFFFFFu);
    return TMS_GPIO_OK;
}
