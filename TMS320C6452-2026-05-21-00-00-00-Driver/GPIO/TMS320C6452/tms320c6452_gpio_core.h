#ifndef TMS320C6452_GPIO_CORE_H
#define TMS320C6452_GPIO_CORE_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Base address (from input contract) */
#define TMS320C6452_GPIO_BASE   (0x18000000u)

/* Register offsets (from RAG) */
#define GPIO_PID_OFFSET                 0x00u
#define GPIO_BINTEN_OFFSET              0x08u
#define GPIO_DIR01_OFFSET               0x10u
#define GPIO_OUT_DATA01_OFFSET          0x14u
#define GPIO_SET_DATA01_OFFSET          0x18u
#define GPIO_CLR_DATA01_OFFSET          0x1Cu
#define GPIO_IN_DATA01_OFFSET           0x20u
#define GPIO_SET_RIS_TRIG01_OFFSET      0x24u
#define GPIO_CLR_RIS_TRIG01_OFFSET      0x28u
#define GPIO_SET_FAL_TRIG01_OFFSET      0x2Cu
#define GPIO_CLR_FAL_TRIG01_OFFSET      0x30u
#define GPIO_INTSTAT01_OFFSET           0x34u

/* Bitfields */
#define GPIO_BINTEN_EN0                 (1u << 0)
#define GPIO_BINTEN_EN1                 (1u << 1)

/* Core interface exposing MMIO helpers to feature drivers */
typedef struct {
    uintptr_t base;                                       /* Base address used for offset addressing */
    uint32_t (*rd)(uint32_t offset);                      /* Read 32-bit from base+offset */
    void     (*wr)(uint32_t offset, uint32_t value);      /* Write 32-bit to base+offset */
} tms320c6452_gpio_core_if_t;

extern const tms320C6452_gpio_core_if_t tms320c6452_gpio_core; /* core instance */

/* Inline wrappers for convenience */
static inline uint32_t tms320c6452_gpio_core_read(uint32_t offset)
{
    return tms320c6452_gpio_core.rd(offset);
}

static inline void tms320c6452_gpio_core_write(uint32_t offset, uint32_t value)
{
    tms320c6452_gpio_core.wr(offset, value);
}

#endif /* TMS320C6452_GPIO_CORE_H */
