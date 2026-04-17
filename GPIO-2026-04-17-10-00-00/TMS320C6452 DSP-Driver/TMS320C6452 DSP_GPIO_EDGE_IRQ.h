/*
 * GPIO_EDGE_IRQ feature driver header for TMS320C6452 DSP
 * Configures per-pin edge-triggered interrupts and bank enable.
 */
#ifndef TMS320C6452_DSP_GPIO_EDGE_IRQ_H
#define TMS320C6452_DSP_GPIO_EDGE_IRQ_H

#ifdef __cplusplus
extern "C" {
#endif

#include "TMS320C6452 DSP_core_drv.h"

#define TMS320C6452_DSP_CORE_BASE      (0xA1008000u)
#define TMS320C6452_DSP_GPIO_BASE      (0x02B00000u)
#define TMS320C6452_DSP_GPIO_OFF(x)    ((u32)((TMS320C6452_DSP_GPIO_BASE - TMS320C6452_DSP_CORE_BASE) + (u32)(x)))

/* Register offsets */
#define GPIO_BINTEN_OFF                (0x08u)
#define GPIO_DIR01_OFF                 (0x10u)
#define GPIO_SET_RIS_TRIG01_OFF        (0x24u)
#define GPIO_CLR_RIS_TRIG01_OFF        (0x28u)
#define GPIO_SET_FAL_TRIG01_OFF        (0x2Cu)
#define GPIO_CLR_FAL_TRIG01_OFF        (0x30u)
#define GPIO_INTSTAT01_OFF             (0x34u)

/* Bit helpers */
#define GPIO_PIN(n)                    (1u << (n))     /* n = 0..31 */
#define GPIO_BANK0_INT_EN              GPIO_PIN(0)
#define GPIO_BANK1_INT_EN              GPIO_PIN(1)

/* API */
int tms320c6452_dsp_gpio_irq_init(unsigned int bank_mask);
int tms320c6452_dsp_gpio_irq_enable_pin(unsigned int pin, int rising, int falling);
int tms320c6452_dsp_gpio_irq_disable_pin(unsigned int pin);
unsigned int tms320c6452_dsp_gpio_irq_status(void);
void tms320c6452_dsp_gpio_irq_clear(unsigned int mask);

/* ISR prototype (application integrates with platform INTC; mapping UNKNOWN) */
void tms320c6452_dsp_gpio_isr(void);

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_DSP_GPIO_EDGE_IRQ_H */
