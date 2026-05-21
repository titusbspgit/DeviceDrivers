#ifndef TMS320C6452_GPIO_CORE_H
#define TMS320C6452_GPIO_CORE_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TMS320C6452_GPIO_BASE   ((uintptr_t)0x18000000u)

#define GPIO_PID_OFFSET             (0x00u)
#define GPIO_BINTEN_OFFSET          (0x08u)
#define GPIO_DIR01_OFFSET           (0x10u)
#define GPIO_OUT_DATA01_OFFSET      (0x14u)
#define GPIO_SET_DATA01_OFFSET      (0x18u)
#define GPIO_CLR_DATA01_OFFSET      (0x1Cu)
#define GPIO_IN_DATA01_OFFSET       (0x20u)
#define GPIO_SET_RIS_TRIG01_OFFSET  (0x24u)
#define GPIO_CLR_RIS_TRIG01_OFFSET  (0x28u)
#define GPIO_SET_FAL_TRIG01_OFFSET  (0x2Cu)
#define GPIO_CLR_FAL_TRIG01_OFFSET  (0x30u)
#define GPIO_INTSTAT01_OFFSET       (0x34u)

#define GPIO_BIT(n)                 ( (uint32_t)(1u) << (uint32_t)(n) )
#define GPIO_BANK_FROM_PIN(n)       ( (uint32_t)(n) >> 4 )

typedef struct {
    uintptr_t base;
    uint32_t (*read32)(uintptr_t base, uint32_t offset);
    void     (*write32)(uintptr_t base, uint32_t offset, uint32_t val);
} tms320c6452_gpio_core_t;

static inline void tms320c6452_gpio_dmb(void)
{
#if defined(__arm__) || defined(__aarch64__)
    __asm__ volatile("dmb sy" ::: "memory");
    __asm__ volatile("isb sy" ::: "memory");
#else
    __asm__ volatile("" ::: "memory");
#endif
}

static inline uint32_t tms320c6452_gpio_core_read(uintptr_t base, uint32_t offset)
{
    tms320c6452_gpio_dmb();
    volatile uint32_t *addr = (volatile uint32_t *)(base + (uintptr_t)offset);
    uint32_t v = *addr;
    tms320c6452_gpio_dmb();
    return v;
}

static inline void tms320c6452_gpio_core_write(uintptr_t base, uint32_t offset, uint32_t val)
{
    tms320c6452_gpio_dmb();
    volatile uint32_t *addr = (volatile uint32_t *)(base + (uintptr_t)offset);
    *addr = val;
    tms320c6452_gpio_dmb();
}

const tms320c6452_gpio_core_t* tms320c6452_gpio_core_get(void);

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_GPIO_CORE_H */
