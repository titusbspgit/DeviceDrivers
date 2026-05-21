#ifndef TMS320C6452_GPIO_CORE_H
#define TMS320C6452_GPIO_CORE_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Target Environment
 * CPU/Arch: arm
 * OS/Baremetal: uboot
 * Compiler: gcc
 * Endianness: big
 */

/* Base address (global configuration) */
#define TMS320C6452_GPIO_BASE   (0x18000000u)

/* Register Offsets (from RAG) */
#define TMS320C6452_GPIO_PID_OFS              (0x00u)
#define TMS320C6452_GPIO_BINTEN_OFS           (0x08u)
#define TMS320C6452_GPIO_DIR01_OFS            (0x10u)
#define TMS320C6452_GPIO_OUT_DATA01_OFS       (0x14u)
#define TMS320C6452_GPIO_SET_DATA01_OFS       (0x18u)
#define TMS320C6452_GPIO_CLR_DATA01_OFS       (0x1Cu)
#define TMS320C6452_GPIO_IN_DATA01_OFS        (0x20u)
#define TMS320C6452_GPIO_SET_RIS_TRIG01_OFS   (0x24u)
#define TMS320C6452_GPIO_CLR_RIS_TRIG01_OFS   (0x28u)
#define TMS320C6452_GPIO_SET_FAL_TRIG01_OFS   (0x2Cu)
#define TMS320C6452_GPIO_CLR_FAL_TRIG01_OFS   (0x30u)
#define TMS320C6452_GPIO_INTSTAT01_OFS        (0x34u)

/* MMIO helpers */
#define TMS320C6452_GPIO_REG32(off)   (*(volatile uint32_t *)((uintptr_t)(TMS320C6452_GPIO_BASE) + (uint32_t)(off)))

/* Register accessors (as lvalues) */
#define TMS320C6452_GPIO_PID              TMS320C6452_GPIO_REG32(TMS320C6452_GPIO_PID_OFS)
#define TMS320C6452_GPIO_BINTEN           TMS320C6452_GPIO_REG32(TMS320C6452_GPIO_BINTEN_OFS)
#define TMS320C6452_GPIO_DIR01            TMS320C6452_GPIO_REG32(TMS320C6452_GPIO_DIR01_OFS)
#define TMS320C6452_GPIO_OUT_DATA01       TMS320C6452_GPIO_REG32(TMS320C6452_GPIO_OUT_DATA01_OFS)
#define TMS320C6452_GPIO_SET_DATA01       TMS320C6452_GPIO_REG32(TMS320C6452_GPIO_SET_DATA01_OFS)
#define TMS320C6452_GPIO_CLR_DATA01       TMS320C6452_GPIO_REG32(TMS320C6452_GPIO_CLR_DATA01_OFS)
#define TMS320C6452_GPIO_IN_DATA01        TMS320C6452_GPIO_REG32(TMS320C6452_GPIO_IN_DATA01_OFS)
#define TMS320C6452_GPIO_SET_RIS_TRIG01   TMS320C6452_GPIO_REG32(TMS320C6452_GPIO_SET_RIS_TRIG01_OFS)
#define TMS320C6452_GPIO_CLR_RIS_TRIG01   TMS320C6452_GPIO_REG32(TMS320C6452_GPIO_CLR_RIS_TRIG01_OFS)
#define TMS320C6452_GPIO_SET_FAL_TRIG01   TMS320C6452_GPIO_REG32(TMS320C6452_GPIO_SET_FAL_TRIG01_OFS)
#define TMS320C6452_GPIO_CLR_FAL_TRIG01   TMS320C6452_GPIO_REG32(TMS320C6452_GPIO_CLR_FAL_TRIG01_OFS)
#define TMS320C6452_GPIO_INTSTAT01        TMS320C6452_GPIO_REG32(TMS320C6452_GPIO_INTSTAT01_OFS)

/* Bit helpers */
#define TMS320C6452_GPIO_PIN_MASK(pin)      ( (uint32_t)1u << (uint32_t)(pin) )      /* pin: 0..31 */
#define TMS320C6452_GPIO_BANK_FROM_PIN(pin) ( (uint32_t)(pin) >> 4 )                 /* 0 for 0..15, 1 for 16..31 */
#define TMS320C6452_GPIO_BANK_MASK(bank)    ( (uint32_t)1u << (uint32_t)(bank) )     /* bank: 0..1 */

/* Core register access interface */
typedef struct {
    void     (*write32)(uint32_t off, uint32_t val);
    uint32_t (*read32)(uint32_t off);
    void     (*set_bits)(uint32_t off, uint32_t mask);
    void     (*clr_bits)(uint32_t off, uint32_t mask);
} tms320c6452_gpio_core_if_t;

void     tms320c6452_gpio_write32(uint32_t off, uint32_t val);
uint32_t tms320c6452_gpio_read32(uint32_t off);
void     tms320c6452_gpio_set_bits(uint32_t off, uint32_t mask);
void     tms320c6452_gpio_clr_bits(uint32_t off, uint32_t mask);

extern const tms320c6452_gpio_core_if_t tms320c6452_gpio_core;

#endif /* TMS320C6452_GPIO_CORE_H */
