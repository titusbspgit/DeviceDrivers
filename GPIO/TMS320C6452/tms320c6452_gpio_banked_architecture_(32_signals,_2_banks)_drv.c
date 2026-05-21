#include "tms320c6452_gpio_banked_architecture_(32_signals,_2_banks).h"

/* Clear all pending GPIO interrupts (write-1-to-clear) */
void tms320c6452_gpio_banked_clear_all_interrupts(tms320c6452_gpio_core_t *core)
{
    if (!core) return;
    gpio_mmio_writel(GPIO_REG_ADDR(core, GPIO_INTSTAT01_OFS), 0xFFFFFFFFu);
}

/* Configure pin direction */
int tms320c6452_gpio_banked_set_dir(tms320c6452_gpio_core_t *core, uint8_t pin, bool is_input)
{
    if (!core || !GPIO_PIN_VALID(pin)) return -1;
    uintptr_t addr = GPIO_REG_ADDR(core, GPIO_DIR01_OFS);
    uint32_t v = gpio_mmio_readl(addr);
    if (is_input) {
        v |= GPIO_PIN_MASK(pin);   /* 1 = input */
    } else {
        v &= ~GPIO_PIN_MASK(pin);  /* 0 = output */
    }
    gpio_mmio_writel(addr, v);
    return 0;
}

int tms320c6452_gpio_banked_get_dir(tms320c6452_gpio_core_t *core, uint8_t pin)
{
    if (!core || !GPIO_PIN_VALID(pin)) return -1;
    uintptr_t addr = GPIO_REG_ADDR(core, GPIO_DIR01_OFS);
    uint32_t v = gpio_mmio_readl(addr);
    return ( (v & GPIO_PIN_MASK(pin)) ? 1 : 0 );
}

/* Drive output pin using atomic set/clear */
int tms320c6452_gpio_banked_write_pin(tms320c6452_gpio_core_t *core, uint8_t pin, bool high)
{
    if (!core || !GPIO_PIN_VALID(pin)) return -1;
    uintptr_t set_addr = GPIO_REG_ADDR(core, GPIO_SET_DATA01_OFS);
    uintptr_t clr_addr = GPIO_REG_ADDR(core, GPIO_CLR_DATA01_OFS);
    uint32_t m = GPIO_PIN_MASK(pin);
    if (high) {
        gpio_mmio_writel(set_addr, m);
    } else {
        gpio_mmio_writel(clr_addr, m);
    }
    return 0;
}

/* Read pin level */
int tms320c6452_gpio_banked_read_pin(tms320c6452_gpio_core_t *core, uint8_t pin)
{
    if (!core || !GPIO_PIN_VALID(pin)) return -1;
    uintptr_t addr = GPIO_REG_ADDR(core, GPIO_IN_DATA01_OFS);
    uint32_t v = gpio_mmio_readl(addr);
    return ( (v & GPIO_PIN_MASK(pin)) ? 1 : 0 );
}

/* Enable/disable per-bank interrupt output */
int tms320c6452_gpio_banked_enable_bank_irq(tms320c6452_gpio_core_t *core, uint8_t bank, bool enable)
{
    if (!core || !GPIO_BANK_VALID(bank)) return -1;
    uintptr_t addr = GPIO_REG_ADDR(core, GPIO_BINTEN_OFS);
    uint32_t v = gpio_mmio_readl(addr);
    uint32_t m = (uint32_t)1u << (uint32_t)bank; /* Bit0:Bank0, Bit1:Bank1 */
    if (enable) {
        v |= m;
    } else {
        v &= ~m;
    }
    gpio_mmio_writel(addr, v);
    return 0;
}

/* Configure edge-detect enables for a pin */
int tms320c6452_gpio_banked_config_edge(tms320c6452_gpio_core_t *core, uint8_t pin, bool rising_en, bool falling_en)
{
    if (!core || !GPIO_PIN_VALID(pin)) return -1;
    uint32_t m = GPIO_PIN_MASK(pin);
    if (rising_en) {
        gpio_mmio_writel(GPIO_REG_ADDR(core, GPIO_SET_RIS_TRIG01_OFS), m);
    } else {
        gpio_mmio_writel(GPIO_REG_ADDR(core, GPIO_CLR_RIS_TRIG01_OFS), m);
    }
    if (falling_en) {
        gpio_mmio_writel(GPIO_REG_ADDR(core, GPIO_SET_FAL_TRIG01_OFS), m);
    } else {
        gpio_mmio_writel(GPIO_REG_ADDR(core, GPIO_CLR_FAL_TRIG01_OFS), m);
    }
    return 0;
}

uint32_t tms320c6452_gpio_banked_get_intstatus(tms320c6452_gpio_core_t *core)
{
    if (!core) return 0u;
    return gpio_mmio_readl(GPIO_REG_ADDR(core, GPIO_INTSTAT01_OFS));
}

void tms320c6452_gpio_banked_clear_intstatus(tms320c6452_gpio_core_t *core, uint32_t mask)
{
    if (!core) return;
    gpio_mmio_writel(GPIO_REG_ADDR(core, GPIO_INTSTAT01_OFS), mask);
}

void tms320c6452_gpio_banked_bank_isr(tms320c6452_gpio_core_t *core, uint32_t *pending_out)
{
    if (!core) return;
    uint32_t pending = gpio_mmio_readl(GPIO_REG_ADDR(core, GPIO_INTSTAT01_OFS));
    if (pending_out) { *pending_out = pending; }
    if (pending) {
        /* Write-1-to-clear for all serviced bits */
        gpio_mmio_writel(GPIO_REG_ADDR(core, GPIO_INTSTAT01_OFS), pending);
    }
}
