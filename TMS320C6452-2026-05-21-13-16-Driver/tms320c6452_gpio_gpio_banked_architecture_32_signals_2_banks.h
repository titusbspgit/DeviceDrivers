#ifndef TMS320C6452_GPIO_BANKED_ARCH_H
#define TMS320C6452_GPIO_BANKED_ARCH_H

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

/* Register offsets */
#define GPIO_PID_OFFSET                (0x00u)
#define GPIO_BINTEN_OFFSET             (0x08u)
#define GPIO_DIR01_OFFSET              (0x10u)
#define GPIO_OUT_DATA01_OFFSET         (0x14u)
#define GPIO_SET_DATA01_OFFSET         (0x18u)
#define GPIO_CLR_DATA01_OFFSET         (0x1Cu)
#define GPIO_IN_DATA01_OFFSET          (0x20u)
#define GPIO_SET_RIS_TRIG01_OFFSET     (0x24u)
#define GPIO_CLR_RIS_TRIG01_OFFSET     (0x28u)
#define GPIO_SET_FAL_TRIG01_OFFSET     (0x2Cu)
#define GPIO_CLR_FAL_TRIG01_OFFSET     (0x30u)
#define GPIO_INTSTAT01_OFFSET          (0x34u)

/* Register addresses */
#define GPIO_BINTEN                    (TMS320C6452_GPIO_BASE + GPIO_BINTEN_OFFSET)
#define GPIO_DIR01                     (TMS320C6452_GPIO_BASE + GPIO_DIR01_OFFSET)
#define GPIO_OUT_DATA01                (TMS320C6452_GPIO_BASE + GPIO_OUT_DATA01_OFFSET)
#define GPIO_SET_DATA01                (TMS320C6452_GPIO_BASE + GPIO_SET_DATA01_OFFSET)
#define GPIO_CLR_DATA01                (TMS320C6452_GPIO_BASE + GPIO_CLR_DATA01_OFFSET)
#define GPIO_IN_DATA01                 (TMS320C6452_GPIO_BASE + GPIO_IN_DATA01_OFFSET)
#define GPIO_SET_RIS_TRIG01            (TMS320C6452_GPIO_BASE + GPIO_SET_RIS_TRIG01_OFFSET)
#define GPIO_CLR_RIS_TRIG01            (TMS320C6452_GPIO_BASE + GPIO_CLR_RIS_TRIG01_OFFSET)
#define GPIO_SET_FAL_TRIG01            (TMS320C6452_GPIO_BASE + GPIO_SET_FAL_TRIG01_OFFSET)
#define GPIO_CLR_FAL_TRIG01            (TMS320C6452_GPIO_BASE + GPIO_CLR_FAL_TRIG01_OFFSET)
#define GPIO_INTSTAT01                 (TMS320C6452_GPIO_BASE + GPIO_INTSTAT01_OFFSET)

/* Bitfields */
#define GPIO_BINTEN_EN_BANK0           (1u << 0)
#define GPIO_BINTEN_EN_BANK1           (1u << 1)

/* Bank/bit mapping for GP[31:0] */
static inline uint32_t gpio_bank_of(uint32_t gpio)
{
    return (gpio >> 4) & 0x1u; /* 0 or 1 */
}

static inline uint32_t gpio_bit_of(uint32_t gpio)
{
    return (gpio & 0xFu); /* 0..15 */
}

static inline uint32_t gpio_mask_of(uint32_t gpio)
{
    return (1u << (gpio & 0x1Fu)); /* 0..31 within 01-registers */
}

/* API declarations (Banked architecture feature only) */
void tms320c6452_gpio_banked_init(void);
void tms320c6452_gpio_bank_enable_interrupt(uint32_t bank, bool enable);

/* Bulk masked output control */
void tms320c6452_gpio_write_mask(uint32_t set_mask, uint32_t clr_mask);

/* Edge enable helpers */
void tms320c6452_gpio_enable_rising(uint32_t mask);
void tms320c6452_gpio_disable_rising(uint32_t mask);
void tms320c6452_gpio_enable_falling(uint32_t mask);
void tms320c6452_gpio_disable_falling(uint32_t mask);

/* Interrupt status/clear */
uint32_t tms320c6452_gpio_get_int_status(void);
void tms320c6452_gpio_clear_interrupts(uint32_t mask); /* UNKNOWN clear mechanism: stub implementation */

#endif /* TMS320C6452_GPIO_BANKED_ARCH_H */
