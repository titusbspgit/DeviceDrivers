#ifndef TMS320C6452_GPIO_DIRECTION_CONTROL_DIR01_H
#define TMS320C6452_GPIO_DIRECTION_CONTROL_DIR01_H

#include <stdint.h>
#include <stdbool.h>

/* Controller Name: TMS320C6452 GPIO */
/* Base Address: 0x02B00000 */
/* Target Environment: CPU/Arch: UNKNOWN, OS/Baremetal: UNKNOWN, Compiler: UNKNOWN, Endianness: UNKNOWN */

#ifndef HWREG32
#define HWREG32(x) (*((volatile uint32_t *)(x)))
#endif

#define TMS320C6452_GPIO_BASE          (0x02B00000u)

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
    /* For deterministic access use DATA SET/CLR via architecture feature if available; here we directly RMW OUT_DATA01
     * is not defined in this header; user should include banked architecture header for atomic ops if needed. */
    (void)gpio; (void)high; /* Not implemented here to keep feature isolation. */
}

static inline uint32_t tms320c6452_gpio_read_dir(void)
{
    return HWREG32(GPIO_DIR01);
}

#endif /* TMS320C6452_GPIO_DIRECTION_CONTROL_DIR01_H */
