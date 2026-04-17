/*
 * GPIO_INIT feature driver header for TMS320C6452 DSP
 * Single-feature: GPIO_INIT
 * Memory-mapped I/O, Baremetal, gcc, big-endian
 */
#ifndef TMS320C6452_DSP_GPIO_INIT_H
#define TMS320C6452_DSP_GPIO_INIT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "TMS320C6452 DSP_core_drv.h"

/* Core base (from global configuration) */
#define TMS320C6452_DSP_CORE_BASE      (0xA1008000u)

/* GPIO base (from device reference) */
#define TMS320C6452_DSP_GPIO_BASE      (0x02B00000u)

/* Calculate GPIO register access offsets relative to core base to use core ops */
#define TMS320C6452_DSP_GPIO_OFF(x)    ((u32)((TMS320C6452_DSP_GPIO_BASE - TMS320C6452_DSP_CORE_BASE) + (u32)(x)))

/* GPIO register offsets */
#define GPIO_PID_OFF                   (0x00u)
#define GPIO_BINTEN_OFF                (0x08u)
#define GPIO_DIR01_OFF                 (0x10u)
#define GPIO_OUT_DATA01_OFF            (0x14u)
#define GPIO_SET_DATA01_OFF            (0x18u)
#define GPIO_CLR_DATA01_OFF            (0x1Cu)
#define GPIO_IN_DATA01_OFF             (0x20u)
#define GPIO_SET_RIS_TRIG01_OFF        (0x24u)
#define GPIO_CLR_RIS_TRIG01_OFF        (0x28u)
#define GPIO_SET_FAL_TRIG01_OFF        (0x2Cu)
#define GPIO_CLR_FAL_TRIG01_OFF        (0x30u)
#define GPIO_INTSTAT01_OFF             (0x34u)

/* Bit helpers */
#define GPIO_PIN(n)                    (1u << (n))     /* n = 0..31 */
#define GPIO_BANK0_INT_EN              GPIO_PIN(0)     /* BINTEN[0] */
#define GPIO_BANK1_INT_EN              GPIO_PIN(1)     /* BINTEN[1] */

/* Reset values */
#define GPIO_BINTEN_RST                (0x00000000u)
#define GPIO_DIR01_RST                 (0xFFFFFFFFu)
#define GPIO_OUT_DATA01_RST            (0x00000000u)
#define GPIO_INTSTAT01_RST             (0x00000000u)

/* API: Initialize GPIO controller to safe defaults (all inputs, interrupts disabled) */
int tms320c6452_dsp_gpio_init(void);

/* Optional readbacks for verification */
unsigned int tms320c6452_dsp_gpio_read_dir(void);
unsigned int tms320c6452_dsp_gpio_read_in(void);
unsigned int tms320c6452_dsp_gpio_read_binten(void);
unsigned int tms320c6452_dsp_gpio_read_rise_en(void);
unsigned int tms320c6452_dsp_gpio_read_fall_en(void);
unsigned int tms320c6452_dsp_gpio_read_intstat(void);

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_DSP_GPIO_INIT_H */
