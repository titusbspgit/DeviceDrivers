#ifndef TMS320C6452_GPIO_BANKED_ARCHITECTURE_32_SIGNALS_2_BANKS_H
#define TMS320C6452_GPIO_BANKED_ARCHITECTURE_32_SIGNALS_2_BANKS_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Core register base (memory-mapped I/O) */
#define TMS320C6452_GPIO_BASE   ((uintptr_t)0x18000000u)

/* GPIO register offsets (from RAG) */
#define GPIO_PID_OFS            (0x00u)
#define GPIO_BINTEN_OFS         (0x08u)
#define GPIO_DIR01_OFS          (0x10u)
#define GPIO_OUT_DATA01_OFS     (0x14u)
#define GPIO_SET_DATA01_OFS     (0x18u)
#define GPIO_CLR_DATA01_OFS     (0x1Cu)
#define GPIO_IN_DATA01_OFS      (0x20u)
#define GPIO_SET_RIS_TRIG01_OFS (0x24u)
#define GPIO_CLR_RIS_TRIG01_OFS (0x28u)
#define GPIO_SET_FAL_TRIG01_OFS (0x2Cu)
#define GPIO_CLR_FAL_TRIG01_OFS (0x30u)
#define GPIO_INTSTAT01_OFS      (0x34u)

/* Banks/pins */
#define GPIO_TOTAL_PINS         (32u)
#define GPIO_BANK0              (0u) /* GP[15:0]  */
#define GPIO_BANK1              (1u) /* GP[31:16] */

#define GPIO_PIN_VALID(pin)     ((uint32_t)(pin) < GPIO_TOTAL_PINS)
#define GPIO_PIN_MASK(pin)      ( (uint32_t)1u << ((uint32_t)(pin) & 31u) )
#define GPIO_BANK_VALID(bank)   (((bank) == GPIO_BANK0) || ((bank) == GPIO_BANK1))

/* Core access helpers (deterministic, MMIO) */
static inline void gpio_mmio_writel(uintptr_t addr, uint32_t val)
{
    *(volatile uint32_t *)(addr) = val;
}

static inline uint32_t gpio_mmio_readl(uintptr_t addr)
{
    return *(volatile uint32_t *)(addr);
}

/* Core descriptor for this feature driver */
typedef struct {
    uintptr_t base; /* MMIO base */
} tms320c6452_gpio_core_t;

/* Initialize core with base address */
static inline void tms320c6452_gpio_core_init(tms320c6452_gpio_core_t *core, uintptr_t base)
{
    if (core) { core->base = base; }
}

/* Address computation */
#define GPIO_REG_ADDR(core_ptr, ofs)   ((uintptr_t)((core_ptr)->base + (uintptr_t)(ofs)))

/* Feature-specific API: GPIO banked architecture (32 signals, 2 banks) */

/* Clear all pending GPIO interrupts (write-1-to-clear) */
void tms320c6452_gpio_banked_clear_all_interrupts(tms320c6452_gpio_core_t *core);

/* Configure pin direction: is_input = true => input (DIR bit = 1), false => output (DIR bit = 0) */
int  tms320c6452_gpio_banked_set_dir(tms320c6452_gpio_core_t *core, uint8_t pin, bool is_input);

/* Read back pin direction: returns 1 for input, 0 for output; negative on error */
int  tms320c6452_gpio_banked_get_dir(tms320c6452_gpio_core_t *core, uint8_t pin);

/* Drive output pin level using atomic set/clear; error if pin invalid */
int  tms320c6452_gpio_banked_write_pin(tms320c6452_gpio_core_t *core, uint8_t pin, bool high);

/* Read pin level from IN_DATA (valid for both input and output pins) */
int  tms320c6452_gpio_banked_read_pin(tms320c6452_gpio_core_t *core, uint8_t pin);

/* Enable/disable per-bank interrupt output (BINTEN[bank]) */
int  tms320c6452_gpio_banked_enable_bank_irq(tms320c6452_gpio_core_t *core, uint8_t bank, bool enable);

/* Configure edge-detect enables for a pin (rising/falling) */
int  tms320c6452_gpio_banked_config_edge(tms320c6452_gpio_core_t *core, uint8_t pin, bool rising_en, bool falling_en);

/* Read INTSTAT bits (pending interrupts) */
uint32_t tms320c6452_gpio_banked_get_intstatus(tms320c6452_gpio_core_t *core);

/* Clear INTSTAT bits (write-1-to-clear) */
void tms320c6452_gpio_banked_clear_intstatus(tms320c6452_gpio_core_t *core, uint32_t mask);

/* Simple bank ISR service: read and clear pending bits, return mask via out pointer if non-NULL */
void tms320c6452_gpio_banked_bank_isr(tms320c6452_gpio_core_t *core, uint32_t *pending_out);

#endif /* TMS320C6452_GPIO_BANKED_ARCHITECTURE_32_SIGNALS_2_BANKS_H */
