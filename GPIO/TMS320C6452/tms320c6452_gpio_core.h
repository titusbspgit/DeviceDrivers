#ifndef TMS320C6452_GPIO_CORE_H
#define TMS320C6452_GPIO_CORE_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* MMIO base address */
#define TMS320C6452_GPIO_BASE   ((uintptr_t)0x18000000u)

/* Register offsets (relative to base) */
#define TMS320C6452_GPIO_PID_OFS             (0x00u)
#define TMS320C6452_GPIO_BINTEN_OFS          (0x08u)
#define TMS320C6452_GPIO_DIR01_OFS           (0x10u)
#define TMS320C6452_GPIO_OUT_DATA01_OFS      (0x14u)
#define TMS320C6452_GPIO_SET_DATA01_OFS      (0x18u)
#define TMS320C6452_GPIO_CLR_DATA01_OFS      (0x1Cu)
#define TMS320C6452_GPIO_IN_DATA01_OFS       (0x20u)
#define TMS320C6452_GPIO_SET_RIS_TRIG01_OFS  (0x24u)
#define TMS320C6452_GPIO_CLR_RIS_TRIG01_OFS  (0x28u)
#define TMS320C6452_GPIO_SET_FAL_TRIG01_OFS  (0x2Cu)
#define TMS320C6452_GPIO_CLR_FAL_TRIG01_OFS  (0x30u)
#define TMS320C6452_GPIO_INTSTAT01_OFS       (0x34u)

/* Core descriptor */
typedef struct {
    uintptr_t base;
} tms320c6452_gpio_core_t;

/* Core initialization */
void tms320c6452_gpio_core_init(tms320c6452_gpio_core_t *core, uintptr_t base);

/* Deterministic MMIO accessors */
void     tms320c6452_gpio_writel(tms320c6452_gpio_core_t *core, uint32_t ofs, uint32_t val);
uint32_t tms320c6452_gpio_readl (tms320c6452_gpio_core_t *core, uint32_t ofs);

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_GPIO_CORE_H */
