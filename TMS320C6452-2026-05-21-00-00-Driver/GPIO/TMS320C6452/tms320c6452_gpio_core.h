#ifndef TMS320C6452_GPIO_CORE_H
#define TMS320C6452_GPIO_CORE_H

#include <stdint.h>
#include <stddef.h>

/* Controller: TMS320C6452 */
/* Base Address: 0x18000000 */
#define TMS320C6452_GPIO_BASE   ((uintptr_t)0x18000000u)

/* Register Offsets (relative to base) */
#define GPIO_BINTEN_OFFSET          0x08u
#define GPIO_DIR01_OFFSET           0x10u
#define GPIO_OUT_DATA01_OFFSET      0x14u
#define GPIO_SET_DATA01_OFFSET      0x18u
#define GPIO_CLR_DATA01_OFFSET      0x1Cu
#define GPIO_IN_DATA01_OFFSET       0x20u
#define GPIO_SET_RIS_TRIG01_OFFSET  0x24u
#define GPIO_CLR_RIS_TRIG01_OFFSET  0x28u
#define GPIO_SET_FAL_TRIG01_OFFSET  0x2Cu
#define GPIO_CLR_FAL_TRIG01_OFFSET  0x30u
#define GPIO_INTSTAT01_OFFSET       0x34u

/* Bitfields - UNKNOWN where not specified */
/* NOTE: Per-bank interrupt enable bits are UNKNOWN in BINTEN for this context */
/* #define GPIO_BINTEN_EN0         UNKNOWN */
/* #define GPIO_BINTEN_EN1         UNKNOWN */

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint32_t (*rd)(uint32_t off);
    void     (*wr)(uint32_t off, uint32_t val);
    uintptr_t base;
} tms320c6452_gpio_core_api_t;

extern const tms320c6452_gpio_core_api_t tms320c6452_gpio_core;

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_GPIO_CORE_H */
