#include "TMS320C6452 DSP_gpio_banked_arch_32x2.h"

/* Static pointer to core ops provided at init */
static const struct gpio_core_ops* s_ops = (const struct gpio_core_ops*)0;

/* Local helpers */
static int32_t gpio_validate_bank(uint32_t bank)
{
    return (bank < 2u) ? GPIO_EOK : GPIO_EINVAL;
}

static uint32_t gpio_bank_to_mask(uint32_t bank, uint32_t mask)
{
    if (bank == 0u) {
        return (mask & 0xFFFFu);
    }
    /* bank 1 */
    return ((mask & 0xFFFFu) << 16);
}

int32_t gpio_arch_init(const struct gpio_core_ops* ops)
{
    if (ops == (const struct gpio_core_ops*)0) {
        return GPIO_EINVAL;
    }
    /* Optional: verify base is non-zero */
    s_ops = ops;
    return GPIO_EOK;
}

int32_t gpio_set_dir_bank(uint32_t bank, uint32_t mask, bool output)
{
    uint32_t mapped;
    uint32_t dir;
    uintptr_t dir_addr;

    if (s_ops == (const struct gpio_core_ops*)0) { return GPIO_EINVAL; }
    if (gpio_validate_bank(bank) != GPIO_EOK) { return GPIO_EINVAL; }

    mapped = gpio_bank_to_mask(bank, mask);
    dir_addr = s_ops->base + (uintptr_t)GPIO_REG_DIR01_OFFSET;

    dir = s_ops->read32(dir_addr);
    if (output) {
        /* direction bit 0=output, 1=input per C6452; clear bits to make outputs */
        dir &= ~mapped;
    } else {
        dir |= mapped;
    }
    s_ops->write32(dir_addr, dir);

    return GPIO_EOK;
}

int32_t gpio_write_bank(uint32_t bank, uint32_t set_mask, uint32_t clr_mask)
{
    uint32_t set_mapped;
    uint32_t clr_mapped;

    if (s_ops == (const struct gpio_core_ops*)0) { return GPIO_EINVAL; }
    if (gpio_validate_bank(bank) != GPIO_EOK) { return GPIO_EINVAL; }

    set_mapped = gpio_bank_to_mask(bank, set_mask);
    clr_mapped = gpio_bank_to_mask(bank, clr_mask);

    if (set_mapped != 0u) {
        s_ops->write32(s_ops->base + (uintptr_t)GPIO_REG_SET_DATA01_OFFSET, set_mapped);
    }
    if (clr_mapped != 0u) {
        s_ops->write32(s_ops->base + (uintptr_t)GPIO_REG_CLR_DATA01_OFFSET, clr_mapped);
    }

    return GPIO_EOK;
}

int32_t gpio_read_bank(uint32_t bank, uint32_t* in_bits)
{
    uint32_t val;

    if (s_ops == (const struct gpio_core_ops*)0) { return GPIO_EINVAL; }
    if (in_bits == (uint32_t*)0) { return GPIO_EINVAL; }
    if (gpio_validate_bank(bank) != GPIO_EOK) { return GPIO_EINVAL; }

    val = s_ops->read32(s_ops->base + (uintptr_t)GPIO_REG_IN_DATA01_OFFSET);

    if (bank == 0u) {
        *in_bits = (val & 0xFFFFu);
    } else {
        *in_bits = ((val >> 16) & 0xFFFFu);
    }

    return GPIO_EOK;
}

int32_t gpio_irq_enable_bank(uint32_t bank, uint32_t mask, bool enable)
{
    uint32_t mapped;

    if (s_ops == (const struct gpio_core_ops*)0) { return GPIO_EINVAL; }
    if (gpio_validate_bank(bank) != GPIO_EOK) { return GPIO_EINVAL; }

    mapped = gpio_bank_to_mask(bank, mask);

    if (enable) {
        /* For generic enable, allow both rising and falling for requested bits */
        s_ops->write32(s_ops->base + (uintptr_t)GPIO_REG_SET_RIS_TRIG01_OFFSET, mapped);
        s_ops->write32(s_ops->base + (uintptr_t)GPIO_REG_SET_FAL_TRIG01_OFFSET, mapped);
        /* Enable bank master bit */
        s_ops->bitset32(s_ops->base + (uintptr_t)GPIO_REG_BINTEN_OFFSET, (uint32_t)(1u << bank));
    } else {
        s_ops->write32(s_ops->base + (uintptr_t)GPIO_REG_CLR_RIS_TRIG01_OFFSET, mapped);
        s_ops->write32(s_ops->base + (uintptr_t)GPIO_REG_CLR_FAL_TRIG01_OFFSET, mapped);
        /* If all per-pin disables were requested, keep bank master unchanged for safety. */
    }

    return GPIO_EOK;
}

int32_t gpio_irq_status_bank(uint32_t bank, uint32_t* status)
{
    uint32_t st;

    if (s_ops == (const struct gpio_core_ops*)0) { return GPIO_EINVAL; }
    if (status == (uint32_t*)0) { return GPIO_EINVAL; }
    if (gpio_validate_bank(bank) != GPIO_EOK) { return GPIO_EINVAL; }

    st = s_ops->read32(s_ops->base + (uintptr_t)GPIO_REG_INTSTAT01_OFFSET);

    if (bank == 0u) {
        *status = (st & 0xFFFFu);
    } else {
        *status = ((st >> 16) & 0xFFFFu);
    }

    /* Clear served bits is left to caller using INTSTAT01 W1C if desired */

    return GPIO_EOK;
}
