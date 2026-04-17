/* RCI-ag-DrvGen Agent: GPIO Direction driver source for TMS320C6452 DSP */
#include "TMS320C6452 DSP_GPIO_DIRECTION.h"

/* Static base pointer */
static volatile TMS320C6452_GPIO_Regs * gpio_regs = (volatile TMS320C6452_GPIO_Regs *)(0xA1008000u);

void gpio_dir_init(uintptr_t base)
{
    /* Initialize base if provided; otherwise keep default */
    if (base != (uintptr_t)0) {
        gpio_regs = (volatile TMS320C6452_GPIO_Regs *)base;
    }
}

/* Internal helpers */
static inline uint32_t read_reg(volatile uint32_t const * addr)
{
    return *addr;
}

static inline void write_reg(volatile uint32_t * addr, uint32_t val)
{
    *addr = val;
}

static inline void data_sync_barrier(void)
{
    __asm__ volatile ("dmb ish" ::: "memory");
}

static inline void instr_sync_barrier(void)
{
    __asm__ volatile ("isb" ::: "memory");
}

gpio_dir_status_t gpio_set_dir(uint32_t pin, bool is_output)
{
    uint32_t mask;
    uint32_t dir;

    if (pin > GPIO_MAX_PIN) {
        return GPIO_DIR_ERR_PARAM;
    }

    mask = (uint32_t)1u << pin;

    /* RMW on DIR01: 1=input, 0=output */
    dir = read_reg(&gpio_regs->DIR01);
    if (is_output) {
        dir &= ~mask; /* clear -> output */
    } else {
        dir |= mask;  /* set -> input */
    }

    data_sync_barrier();
    write_reg(&gpio_regs->DIR01, dir);
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

    dir = read_reg(&gpio_regs->DIR01);
    *is_output = ((dir & ((uint32_t)1u << pin)) == 0u);

    return GPIO_DIR_OK;
}
