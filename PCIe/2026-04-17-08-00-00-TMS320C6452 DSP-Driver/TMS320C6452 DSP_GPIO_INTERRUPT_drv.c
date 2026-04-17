/* RCI-ag-DrvGen Agent: GPIO Interrupt driver source for TMS320C6452 DSP */
#include "TMS320C6452 DSP_GPIO_INTERRUPT.h"

/* Default base per provided configuration */
static uintptr_t gpio_base = (uintptr_t)0xA1008000u;

/* Memory barriers for MMIO ordering */
#if defined(__arm__) || defined(__aarch64__)
static inline void data_sync_barrier(void) { __asm__ volatile ("dmb ish" ::: "memory"); }
static inline void instr_sync_barrier(void) { __asm__ volatile ("isb" ::: "memory"); }
#else
static inline void data_sync_barrier(void) { __sync_synchronize(); }
static inline void instr_sync_barrier(void) { /* no-op */ }
#endif

static inline uint32_t mmio_read32(uintptr_t base, uint32_t offset)
{
    volatile uint32_t const * const p = (volatile uint32_t const *)(base + (uintptr_t)offset);
    return *p;
}

static inline void mmio_write32(uintptr_t base, uint32_t offset, uint32_t value)
{
    volatile uint32_t * const p = (volatile uint32_t *)(base + (uintptr_t)offset);
    *p = value;
}

void gpio_irq_init(uintptr_t base)
{
    if (base != (uintptr_t)0) {
        gpio_base = base;
    }
}

static void cfg_edge(uint32_t pin, bool en_rise, bool en_fall)
{
    const uint32_t mask = GPIO_PIN_MASK(pin);

    if (en_rise) {
        mmio_write32(gpio_base, GPIO_SET_RIS_TRIG01_OFFSET, mask);
    } else {
        mmio_write32(gpio_base, GPIO_CLR_RIS_TRIG01_OFFSET, mask);
    }

    if (en_fall) {
        mmio_write32(gpio_base, GPIO_SET_FAL_TRIG01_OFFSET, mask);
    } else {
        mmio_write32(gpio_base, GPIO_CLR_FAL_TRIG01_OFFSET, mask);
    }
}

gpio_irq_status_t gpio_irq_config(uint32_t pin, gpio_irq_type type)
{
    bool en_rise = false;
    bool en_fall = false;

    if (pin > GPIO_MAX_PIN) {
        return GPIO_IRQ_ERR_PARAM;
    }

    switch (type) {
    case GPIO_IRQ_EDGE_RISE:
        en_rise = true; en_fall = false; break;
    case GPIO_IRQ_EDGE_FALL:
        en_rise = false; en_fall = true; break;
    case GPIO_IRQ_EDGE_BOTH:
        en_rise = true; en_fall = true; break;
    case GPIO_IRQ_LEVEL_HIGH: /* not supported in HW: approximate as rising edge */
        en_rise = true; en_fall = false; break;
    case GPIO_IRQ_LEVEL_LOW:  /* not supported in HW: approximate as falling edge */
        en_rise = false; en_fall = true; break;
    default:
        return GPIO_IRQ_ERR_PARAM;
    }

    data_sync_barrier();
    cfg_edge(pin, en_rise, en_fall);
    data_sync_barrier();
    instr_sync_barrier();

    return GPIO_IRQ_OK;
}

static inline uint32_t bank_bit(uint32_t pin)
{
    /* Bank 0: 0..15 -> EN0; Bank 1: 16..31 -> EN1 */
    (void)pin;
    return 0u; /* not used directly; kept for extension */
}

gpio_irq_status_t gpio_irq_enable(uint32_t pin)
{
    uint32_t binten;

    if (pin > GPIO_MAX_PIN) {
        return GPIO_IRQ_ERR_PARAM;
    }

    /* Enable bank delivery: set EN0 or EN1 depending on pin */
    binten = mmio_read32(gpio_base, GPIO_BINTEN_OFFSET);
    if (pin < 16u) {
        binten |= 0x00000001u; /* EN0 */
    } else {
        binten |= 0x00000002u; /* EN1 */
    }

    data_sync_barrier();
    mmio_write32(gpio_base, GPIO_BINTEN_OFFSET, binten);
    data_sync_barrier();
    instr_sync_barrier();

    return GPIO_IRQ_OK;
}

gpio_irq_status_t gpio_irq_disable(uint32_t pin)
{
    uint32_t binten;

    if (pin > GPIO_MAX_PIN) {
        return GPIO_IRQ_ERR_PARAM;
    }

    binten = mmio_read32(gpio_base, GPIO_BINTEN_OFFSET);
    if (pin < 16u) {
        binten &= ~0x00000001u; /* EN0 */
    } else {
        binten &= ~0x00000002u; /* EN1 */
    }

    data_sync_barrier();
    mmio_write32(gpio_base, GPIO_BINTEN_OFFSET, binten);
    data_sync_barrier();
    instr_sync_barrier();

    return GPIO_IRQ_OK;
}

gpio_irq_status_t gpio_irq_status(uint32_t pin, bool * pending)
{
    uint32_t st;

    if ((pin > GPIO_MAX_PIN) || (pending == (bool *)0)) {
        return GPIO_IRQ_ERR_PARAM;
    }

    st = mmio_read32(gpio_base, GPIO_INTSTAT01_OFFSET);
    *pending = ((st & GPIO_PIN_MASK(pin)) != 0u);
    return GPIO_IRQ_OK;
}

gpio_irq_status_t gpio_irq_clear(uint32_t pin)
{
    if (pin > GPIO_MAX_PIN) {
        return GPIO_IRQ_ERR_PARAM;
    }

    /* INTSTAT01 is W1C: write 1 to clear */
    data_sync_barrier();
    mmio_write32(gpio_base, GPIO_INTSTAT01_OFFSET, GPIO_PIN_MASK(pin));
    data_sync_barrier();
    instr_sync_barrier();

    return GPIO_IRQ_OK;
}
