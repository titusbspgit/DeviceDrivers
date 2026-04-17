/* RCI-ag-DrvGen Agent: GPIO Data I/O driver source for TMS320C6452 DSP */
#include "TMS320C6452 DSP_GPIO_DATA_IO.h"

/* Static base (default per global config) */
static uintptr_t gpio_base = (uintptr_t)0xA1008000u;

/* Barriers for MMIO ordering */
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

void gpio_data_init(uintptr_t base)
{
    if (base != (uintptr_t)0) {
        gpio_base = base;
    }
}

gpio_data_status_t gpio_write(uint32_t pin, bool value)
{
    uint32_t mask;

    if (pin > GPIO_MAX_PIN) {
        return GPIO_DATA_ERR_PARAM;
    }
    mask = GPIO_PIN_MASK(pin);

    data_sync_barrier();
    if (value) {
        mmio_write32(gpio_base, GPIO_SET_DATA01_OFFSET, mask);
    } else {
        mmio_write32(gpio_base, GPIO_CLR_DATA01_OFFSET, mask);
    }
    data_sync_barrier();
    instr_sync_barrier();

    return GPIO_DATA_OK;
}

gpio_data_status_t gpio_set(uint32_t pin)
{
    if (pin > GPIO_MAX_PIN) {
        return GPIO_DATA_ERR_PARAM;
    }
    mmio_write32(gpio_base, GPIO_SET_DATA01_OFFSET, GPIO_PIN_MASK(pin));
    data_sync_barrier();
    instr_sync_barrier();
    return GPIO_DATA_OK;
}

gpio_data_status_t gpio_clear(uint32_t pin)
{
    if (pin > GPIO_MAX_PIN) {
        return GPIO_DATA_ERR_PARAM;
    }
    mmio_write32(gpio_base, GPIO_CLR_DATA01_OFFSET, GPIO_PIN_MASK(pin));
    data_sync_barrier();
    instr_sync_barrier();
    return GPIO_DATA_OK;
}

gpio_data_status_t gpio_toggle(uint32_t pin)
{
    uint32_t mask;
    uint32_t val;

    if (pin > GPIO_MAX_PIN) {
        return GPIO_DATA_ERR_PARAM;
    }

    mask = GPIO_PIN_MASK(pin);
    /* Read current output latch and invert atomically via RMW on OUT_DATA01 */
    val = mmio_read32(gpio_base, GPIO_OUT_DATA01_OFFSET);
    val ^= mask;
    data_sync_barrier();
    mmio_write32(gpio_base, GPIO_OUT_DATA01_OFFSET, val);
    data_sync_barrier();
    instr_sync_barrier();

    return GPIO_DATA_OK;
}

gpio_data_status_t gpio_read_in(uint32_t pin, bool * value)
{
    uint32_t in;

    if ((pin > GPIO_MAX_PIN) || (value == (bool *)0)) {
        return GPIO_DATA_ERR_PARAM;
    }

    in = mmio_read32(gpio_base, GPIO_IN_DATA01_OFFSET);
    *value = ((in & GPIO_PIN_MASK(pin)) != 0u);
    return GPIO_DATA_OK;
}

gpio_data_status_t gpio_read_out(uint32_t pin, bool * value)
{
    uint32_t out;

    if ((pin > GPIO_MAX_PIN) || (value == (bool *)0)) {
        return GPIO_DATA_ERR_PARAM;
    }

    out = mmio_read32(gpio_base, GPIO_OUT_DATA01_OFFSET);
    *value = ((out & GPIO_PIN_MASK(pin)) != 0u);
    return GPIO_DATA_OK;
}
