#ifndef TMS320C6452_DSP_GPIO_BANKED_ARCHITECTURE_32_SIGNALS_2_BANKS_H
#define TMS320C6452_DSP_GPIO_BANKED_ARCHITECTURE_32_SIGNALS_2_BANKS_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TMS320C6452_DSP_GPIO_BANKED_ARCHITECTURE_32_SIGNALS_2_BANKS_BASE   ((uintptr_t)0xA1008000u)

/* Register Offsets (Relative to BASE) */
#define GPIO_PID_OFFSET              (0x00u)
#define GPIO_BINTEN_OFFSET           (0x08u)
#define GPIO_DIR01_OFFSET            (0x10u)
#define GPIO_OUT_DATA01_OFFSET       (0x14u)
#define GPIO_SET_DATA01_OFFSET       (0x18u)
#define GPIO_CLR_DATA01_OFFSET       (0x1Cu)
#define GPIO_IN_DATA01_OFFSET        (0x20u)
#define GPIO_SET_RIS_TRIG01_OFFSET   (0x24u)
#define GPIO_CLR_RIS_TRIG01_OFFSET   (0x28u)
#define GPIO_SET_FAL_TRIG01_OFFSET   (0x2Cu)
#define GPIO_CLR_FAL_TRIG01_OFFSET   (0x30u)
#define GPIO_INTSTAT01_OFFSET        (0x34u)

/* Register access helpers (big-endian safe 32-bit accesses) */
#define GPIO_REG32(off) (*(volatile uint32_t *)(TMS320C6452_DSP_GPIO_BANKED_ARCHITECTURE_32_SIGNALS_2_BANKS_BASE + (uint32_t)(off)))

/* Bit helpers */
#define GPIO_BIT(n)                  ((uint32_t)1u << (uint32_t)(n))
#define GPIO_VALID_PIN(n)            ((uint32_t)(n) < 32u)

/* API */
void tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_init(void);
bool tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_set_direction(uint32_t pin, bool input_mode);
bool tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_write(uint32_t pin, bool level);
int  tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_read_in(uint32_t pin);
int  tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_read_out(uint32_t pin);

/* Interrupt control */
bool tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_irq_enable(uint32_t pin, bool rising, bool falling);
bool tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_irq_disable(uint32_t pin);
uint32_t tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_irq_status(void);
void tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_irq_clear(uint32_t mask);
bool tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_bank_irq_gate(uint32_t bank, bool enable);

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_DSP_GPIO_BANKED_ARCHITECTURE_32_SIGNALS_2_BANKS_H */
