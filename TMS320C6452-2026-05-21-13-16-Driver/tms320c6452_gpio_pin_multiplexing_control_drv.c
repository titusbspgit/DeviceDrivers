#include "tms320c6452_gpio_pin_multiplexing_control.h"

/* Memory-mapped I/O helpers (deterministic, no dynamic allocation) */
static inline uint32_t mmio_read32(volatile uint32_t* addr) {
    return *addr;
}

static inline void mmio_write32(volatile uint32_t* addr, uint32_t val) {
    *addr = val;
    /* Read-back to ensure write completion (posted write barriers on some buses) */
    (void)*addr;
}

/* Internal helper: compute register index, shift, and mask for a given pin index */
static int pin_to_field(uint32_t pin_index, uint32_t* reg_index, uint32_t* shift_bits, uint32_t* field_mask)
{
    /* Validate pointers */
    if (!reg_index || !shift_bits || !field_mask) {
        return TMS320C6452_GPIO_EFAULT;
    }

    /* Optional pin range check if known */
    (void)TMS320C6452_GPIO_MAX_PINS; /* Placeholder reference to indicate dependency */

    /* Derive field geometry (UNKNOWN replaced when data available) */
    const uint32_t field_bits = TMS320C6452_GPIO_PINMUX_FIELD_BITS; /* UNKNOWN without manual */
    const uint32_t reg_width_bits = 32u; /* Assumed 32-bit PINMUX registers */
    const uint32_t fields_per_reg = (reg_width_bits / field_bits);

    *reg_index = (pin_index / fields_per_reg);
    const uint32_t field_index = (pin_index % fields_per_reg);
    *shift_bits = (field_index * field_bits);
    *field_mask = ((field_bits >= 32u) ? 0xFFFFFFFFu : ((1u << field_bits) - 1u)) << *shift_bits;

    return TMS320C6452_GPIO_EOK;
}

int tms320c6452_gpio_pin_multiplexing_control_init(tms320c6452_gpio_pinmux_ctx_t* ctx, uintptr_t syscfg_base)
{
    if (ctx == NULL) {
        return TMS320C6452_GPIO_EFAULT;
    }
    ctx->syscfg_base = syscfg_base;
    return TMS320C6452_GPIO_EOK;
}

int tms320c6452_gpio_pin_multiplexing_control_unlock(const tms320c6452_gpio_pinmux_ctx_t* ctx)
{
    if (ctx == NULL) {
        return TMS320C6452_GPIO_EFAULT;
    }

    /* Device-specific KICK sequence (addresses and keys UNKNOWN without manual) */
    volatile uint32_t* kick0 = (volatile uint32_t*)(uintptr_t)TMS320C6452_GPIO_KICK0_ADDR;
    volatile uint32_t* kick1 = (volatile uint32_t*)(uintptr_t)TMS320C6452_GPIO_KICK1_ADDR;

    mmio_write32(kick0, (uint32_t)TMS320C6452_GPIO_KICK0_UNLOCK_KEY);
    mmio_write32(kick1, (uint32_t)TMS320C6452_GPIO_KICK1_UNLOCK_KEY);

    return TMS320C6452_GPIO_EOK;
}

int tms320c6452_gpio_pin_multiplexing_control_lock(const tms320c6452_gpio_pinmux_ctx_t* ctx)
{
    if (ctx == NULL) {
        return TMS320C6452_GPIO_EFAULT;
    }

    volatile uint32_t* kick0 = (volatile uint32_t*)(uintptr_t)TMS320C6452_GPIO_KICK0_ADDR;
    volatile uint32_t* kick1 = (volatile uint32_t*)(uintptr_t)TMS320C6452_GPIO_KICK1_ADDR;

    mmio_write32(kick0, (uint32_t)TMS320C6452_GPIO_KICK_LOCK_KEY);
    mmio_write32(kick1, (uint32_t)TMS320C6452_GPIO_KICK_LOCK_KEY);

    return TMS320C6452_GPIO_EOK;
}

int tms320c6452_gpio_pin_multiplexing_control_set(const tms320C6452_gpio_pinmux_ctx_t* ctx, uint32_t pin_index, uint32_t function_sel)
{
    if (ctx == NULL) {
        return TMS320C6452_GPIO_EFAULT;
    }

    uint32_t reg_index, shift_bits, field_mask;
    int rc = pin_to_field(pin_index, &reg_index, &shift_bits, &field_mask);
    if (rc != TMS320C6452_GPIO_EOK) {
        return rc;
    }

    volatile uint32_t* reg = TMS320C6452_GPIO_PINMUX_REG_ADDR(ctx->syscfg_base, reg_index);
    uint32_t val = mmio_read32(reg);

    /* Clear field, set new function selection */
    val &= ~field_mask;
    val |= ((function_sel << shift_bits) & field_mask);

    mmio_write32(reg, val);
    (void)mmio_read32(reg); /* enforce ordering */

    return TMS320C6452_GPIO_EOK;
}

int tms320c6452_gpio_pin_multiplexing_control_get(const tms320c6452_gpio_pinmux_ctx_t* ctx, uint32_t pin_index, uint32_t* out_function_sel)
{
    if (ctx == NULL || out_function_sel == NULL) {
        return TMS320C6452_GPIO_EFAULT;
    }

    uint32_t reg_index, shift_bits, field_mask;
    int rc = pin_to_field(pin_index, &reg_index, &shift_bits, &field_mask);
    if (rc != TMS320C6452_GPIO_EOK) {
        return rc;
    }

    volatile uint32_t* reg = TMS320C6452_GPIO_PINMUX_REG_ADDR(ctx->syscfg_base, reg_index);
    uint32_t val = mmio_read32(reg);

    *out_function_sel = (val & field_mask) >> shift_bits;
    return TMS320C6452_GPIO_EOK;
}
