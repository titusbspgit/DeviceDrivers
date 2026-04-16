// SPDX-License-Identifier: MIT
// RCI-ag-DrvGen Agent
// TMS320C6452 DSP GPIO Driver Implementation
#include "gpio.h"

// Weak default platform hooks (can be overridden by application/test)
__attribute__((weak)) void tmsgpio_platform_enable_psc(void) { (void)0; }
__attribute__((weak)) void tmsgpio_platform_disable_psc(void) { (void)0; }
__attribute__((weak)) void tmsgpio_platform_configure_pinmux(void) { (void)0; }

static inline uintptr_t reg_addr(const tmsgpio_t *ctx, uint32_t offset)
{
    return (ctx->base + (uintptr_t)offset);
}

static inline bool pin_valid(uint32_t pin)
{
    return (pin < 32u);
}

int tmsgpio_init(tmsgpio_t *ctx, uintptr_t base, bool perform_platform_init)
{
    if (ctx == (tmsgpio_t *)0) {
        return TMSGPIO_ERR_INVAL;
    }

    ctx->base = (base != (uintptr_t)0) ? base : (uintptr_t)TMS320C6452_GPIO_BASE_DEFAULT;

    if (perform_platform_init) {
        tmsgpio_platform_configure_pinmux();
        tmsgpio_platform_enable_psc();
    }

    // Clear any pending interrupts
    rci_reg_write32(reg_addr(ctx, GPIO_INTSTAT01_OFFSET), 0xFFFFFFFFu);
    return TMSGPIO_OK;
}

int tmsgpio_get_pid(const tmsgpio_t *ctx, uint32_t *out_pid)
{
    if ((ctx == (const tmsgpio_t *)0) || (out_pid == (uint32_t *)0)) {
        return TMSGPIO_ERR_INVAL;
    }
    *out_pid = rci_reg_read32(reg_addr(ctx, GPIO_PID_OFFSET));
    return TMSGPIO_OK;
}

int tmsgpio_config_pin(const tmsgpio_t *ctx, uint32_t pin, tmsgpio_dir_t dir)
{
    if ((ctx == (const tmsgpio_t *)0) || !pin_valid(pin)) {
        return TMSGPIO_ERR_INVAL;
    }
    uint32_t dir01 = rci_reg_read32(reg_addr(ctx, GPIO_DIR01_OFFSET));
    if (dir == TMSGPIO_DIR_INPUT) {
        dir01 |= GPIO_PIN_BIT(pin);
    } else {
        dir01 &= (uint32_t)(~GPIO_PIN_BIT(pin));
    }
    rci_reg_write32(reg_addr(ctx, GPIO_DIR01_OFFSET), dir01);
    return TMSGPIO_OK;
}

int tmsgpio_config_mask(const tmsgpio_t *ctx, uint32_t dir_mask)
{
    if (ctx == (const tmsgpio_t *)0) {
        return TMSGPIO_ERR_INVAL;
    }
    // Directly program mask: 1=input, 0=output
    rci_reg_write32(reg_addr(ctx, GPIO_DIR01_OFFSET), dir_mask);
    return TMSGPIO_OK;
}

int tmsgpio_write_pin(const tmsgpio_t *ctx, uint32_t pin, bool high)
{
    if ((ctx == (const tmsgpio_t *)0) || !pin_valid(pin)) {
        return TMSGPIO_ERR_INVAL;
    }
    const uintptr_t set_addr = reg_addr(ctx, high ? GPIO_SET_DATA01_OFFSET : GPIO_CLR_DATA01_OFFSET);
    rci_reg_write32(set_addr, GPIO_PIN_BIT(pin));
    return TMSGPIO_OK;
}

int tmsgpio_set_mask(const tmsgpio_t *ctx, uint32_t mask)
{
    if (ctx == (const tmsgpio_t *)0) { return TMSGPIO_ERR_INVAL; }
    rci_reg_write32(reg_addr(ctx, GPIO_SET_DATA01_OFFSET), mask);
    return TMSGPIO_OK;
}

int tmsgpio_clear_mask(const tmsgpio_t *ctx, uint32_t mask)
{
    if (ctx == (const tmsgpio_t *)0) { return TMSGPIO_ERR_INVAL; }
    rci_reg_write32(reg_addr(ctx, GPIO_CLR_DATA01_OFFSET), mask);
    return TMSGPIO_OK;
}

int tmsgpio_read_pin(const tmsgpio_t *ctx, uint32_t pin, bool *level)
{
    if ((ctx == (const tmsgpio_t *)0) || !pin_valid(pin) || (level == (bool *)0)) {
        return TMSGPIO_ERR_INVAL;
    }
    const uint32_t in = rci_reg_read32(reg_addr(ctx, GPIO_IN_DATA01_OFFSET));
    *level = ((in & GPIO_PIN_BIT(pin)) != 0u);
    return TMSGPIO_OK;
}

int tmsgpio_read_port(const tmsgpio_t *ctx, uint32_t *port_bits)
{
    if ((ctx == (const tmsgpio_t *)0) || (port_bits == (uint32_t *)0)) {
        return TMSGPIO_ERR_INVAL;
    }
    *port_bits = rci_reg_read32(reg_addr(ctx, GPIO_IN_DATA01_OFFSET));
    return TMSGPIO_OK;
}

int tmsgpio_irq_config_edge(const tmsgpio_t *ctx, uint32_t pin, bool rising_enable, bool falling_enable)
{
    if ((ctx == (const tmsgpio_t *)0) || !pin_valid(pin)) {
        return TMSGPIO_ERR_INVAL;
    }
    const uint32_t bit = GPIO_PIN_BIT(pin);
    if (rising_enable) {
        rci_reg_write32(reg_addr(ctx, GPIO_SET_RIS_TRIG01_OFFSET), bit);
    } else {
        rci_reg_write32(reg_addr(ctx, GPIO_CLR_RIS_TRIG01_OFFSET), bit);
    }

    if (falling_enable) {
        rci_reg_write32(reg_addr(ctx, GPIO_SET_FAL_TRIG01_OFFSET), bit);
    } else {
        rci_reg_write32(reg_addr(ctx, GPIO_CLR_FAL_TRIG01_OFFSET), bit);
    }
    return TMSGPIO_OK;
}

int tmsgpio_irq_bank_enable(const tmsgpio_t *ctx, uint32_t bank, bool enable)
{
    if (ctx == (const tmsgpio_t *)0) { return TMSGPIO_ERR_INVAL; }
    if (bank > 1u) { return TMSGPIO_ERR_INVAL; }

    uint32_t binten = rci_reg_read32(reg_addr(ctx, GPIO_BINTEN_OFFSET));
    const uint32_t m = (bank == 0u) ? GPIO_BINTEN_EN0_MASK : GPIO_BINTEN_EN1_MASK;
    if (enable) {
        binten |= m;
    } else {
        binten &= (uint32_t)(~m);
    }
    rci_reg_write32(reg_addr(ctx, GPIO_BINTEN_OFFSET), binten);
    return TMSGPIO_OK;
}

int tmsgpio_irq_status(const tmsgpio_t *ctx, uint32_t *status_bits)
{
    if ((ctx == (const tmsgpio_t *)0) || (status_bits == (uint32_t *)0)) {
        return TMSGPIO_ERR_INVAL;
    }
    *status_bits = rci_reg_read32(reg_addr(ctx, GPIO_INTSTAT01_OFFSET));
    return TMSGPIO_OK;
}

int tmsgpio_irq_clear(const tmsgpio_t *ctx, uint32_t mask)
{
    if (ctx == (const tmsgpio_t *)0) { return TMSGPIO_ERR_INVAL; }
    // W1C: write 1s to clear
    rci_reg_write32(reg_addr(ctx, GPIO_INTSTAT01_OFFSET), mask);
    return TMSGPIO_OK;
}

int tmsgpio_enter_low_power(const tmsgpio_t *ctx)
{
    (void)ctx; // state is retained in HW
    tmsgpio_platform_disable_psc();
    return TMSGPIO_OK;
}

int tmsgpio_exit_low_power(const tmsgpio_t *ctx)
{
    (void)ctx;
    tmsgpio_platform_enable_psc();
    return TMSGPIO_OK;
}
