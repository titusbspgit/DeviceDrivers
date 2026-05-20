/* RCI-ag-DrvGen Agent: GPIO Direction driver source for TMS320C6452 DSP */
#include "TMS320C6452 DSP_GPIO_DIRECTION.h"

/*
 * Assumptions (from RAG):
 * - Only a single DIR01 register controls direction; no DIR_SET/DIR_CLR exist.
 * - DIR bit meaning: 1 = input, 0 = output. Reset: all 1s (inputs).
 */

/* Static base address; default per configuration */
static uintptr_t gpio_base = (uintptr_t)0xA1008000u;

/* Barrier helpers (GCC/ARM); fall back to compiler barrier if unavailable */
#if defined(__arm__) || defined(__aarch64__)
static inline void data_sync_barrier(void) { __asm__ volatile ("dmb ish" ::: "memory"); }
static inline void instr_sync_barrier(void) { __asm__ volatile ("isb" ::: "memory"); }
#else
static inline void data_sync_barrier(void) { __sync_synchronize(); }
static inline void instr_sync_barrier(void) { /* no-op */ }
#endif

/* MMIO accessors */
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

void gpio_dir_init(uintptr_t base)
{
    if (base != (uintptr_t)0) {
        gpio_base = base;
    }
}

gpio_dir_status_t gpio_set_dir(uint32_t pin, bool is_output)
{
    uint32_t mask;
    uint32_t dir;

    if (pin > GPIO_MAX_PIN) {
        return GPIO_DIR_ERR_PARAM;
    }

    mask = GPIO_PIN_MASK(pin);

    /* Read–modify–write DIR01 (1=input, 0=output) */
    dir = mmio_read32(gpio_base, GPIO_DIR01_OFFSET);
    if (is_output) {
        dir &= (uint32_t)(~mask); /* clear -> output */
    } else {
        dir |= mask;              /* set   -> input  */
    }

    data_sync_barrier();
    mmio_write32(gpio_base, GPIO_DIR01_OFFSET, dir);
    data_sync_barrier();
    instr_sync_barrier();

    return GPIO_DIR_OK;
}

gpio_dir_status_t gpio_get_dir(uint32_t pin, bool * is_output)
{
    uint32_t dir;

    if ((pin > GPIO_MAX_PIN) || (is_output == (bool *)0)) {
        return GPIO_DIR_ERR_PARAM;
    }

    dir = mmio_read32(gpio_base, GPIO_DIR01_OFFSET);
    *is_output = ((dir & GPIO_PIN_MASK(pin)) == 0u);

    return GPIO_DIR_OK;
}
