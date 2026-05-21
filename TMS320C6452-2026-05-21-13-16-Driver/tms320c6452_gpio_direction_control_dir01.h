#ifndef TMS320C6452_GPIO_DIRECTION_CONTROL_DIR01_H
#define TMS320C6452_GPIO_DIRECTION_CONTROL_DIR01_H

#include <stdint.h>
#include <stdbool.h>

/* Controller Name: TMS320C6452 GPIO */
/* Base Address: configurable; default UNKNOWN if not defined */
/* Target Environment: CPU/Arch: UNKNOWN, OS/Baremetal: UNKNOWN, Compiler: UNKNOWN, Endianness: UNKNOWN */

#ifndef HWREG32
#define HWREG32(x) (*((volatile uint32_t *)(x)))
#endif

#ifndef TMS320C6452_GPIO_BASE
#define TMS320C6452_GPIO_BASE          (0xFFFFFFFFu) /* UNKNOWN */
#endif

/* Register offsets used */
#define GPIO_DIR01_OFFSET              (0x10u)

/* Register addresses */
#define GPIO_DIR01                     (TMS320C6452_GPIO_BASE + GPIO_DIR01_OFFSET)

/* DIR bit behavior: 1 = Input (reset default), 0 = Output */

static inline void tms320c6452_gpio_set_dir_input(uint32_t gpio)
{
    uint32_t mask = (1u << (gpio & 0x1Fu));
    uint32_t v = HWREG32(GPIO_DIR01);
    v |= mask; /* 1 = input */
    HWREG32(GPIO_DIR01) = v;
}

static inline void tms320c6452_gpio_set_dir_output(uint32_t gpio)
{
    uint32_t mask = (1u << (gpio & 0x1Fu));
    uint32_t v = HWREG32(GPIO_DIR01);
    v &= ~mask; /* 0 = output */
    HWREG32(GPIO_DIR01) = v;
}

static inline void tms320c6452_gpio_write_output(uint32_t gpio, bool high)
{
    (void)gpio; (void)high; /* UNKNOWN: OUT_DATA01/SET/CLR registers are outside this feature scope. */
}

static inline uint32_t tms320c6452_gpio_read_dir(void)
{
    return HWREG32(GPIO_DIR01);
}

#endif /* TMS320C6452_GPIO_DIRECTION_CONTROL_DIR01_H */
