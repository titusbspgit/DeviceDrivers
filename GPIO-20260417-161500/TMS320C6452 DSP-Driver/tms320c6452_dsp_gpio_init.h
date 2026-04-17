/*
 * TMS320C6452 DSP - GPIO_INIT feature driver (header)
 * Environment: U-Boot, gcc, big-endian
 * Uses core ops for deterministic MMIO access.
 */
#ifndef TMS320C6452_DSP_GPIO_INIT_H
#define TMS320C6452_DSP_GPIO_INIT_H

#include <stdint.h>
#include "TMS320C6452 DSP_core_drv.h"

#ifdef __cplusplus
extern "C" {
#endif

/* GPIO module base (assumed from device map) */
#define TMS320C6452_DSP_GPIO_BASE        (0x02B00000u)

/* Compute offset to feed core ops (absolute = core_base + this_offset) */
#define TMS320C6452_DSP_GPIO_OFF(x) ((uint32_t)((TMS320C6452_DSP_GPIO_BASE - TMS320C6452_DSP_BASE) + (uint32_t)(x)))

/* Register offsets (32-bit) */
#define GPIO_BINTEN_OFF                  (0x08u)
#define GPIO_DIR01_OFF                   (0x10u)
#define GPIO_OUT_DATA01_OFF              (0x14u)
#define GPIO_SET_DATA01_OFF              (0x18u)
#define GPIO_CLR_DATA01_OFF              (0x1Cu)
#define GPIO_IN_DATA01_OFF               (0x20u)
#define GPIO_SET_RIS_TRIG01_OFF          (0x24u)
#define GPIO_CLR_RIS_TRIG01_OFF          (0x28u)
#define GPIO_SET_FAL_TRIG01_OFF          (0x2Cu)
#define GPIO_CLR_FAL_TRIG01_OFF          (0x30u)
#define GPIO_INTSTAT01_OFF               (0x34u)

/* API */
int tms320c6452_dsp_gpio_init(void);
uint32_t tms320c6452_dsp_gpio_read_dir(void);
uint32_t tms320c6452_dsp_gpio_read_in(void);
uint32_t tms320c6452_dsp_gpio_read_binten(void);
uint32_t tms320c6452_dsp_gpio_read_rise_en(void);
uint32_t tms320c6452_dsp_gpio_read_fall_en(void);
uint32_t tms320c6452_dsp_gpio_read_intstat(void);
void tms320c6452_dsp_gpio_write_output_mask(uint32_t set_mask, uint32_t clr_mask);

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_DSP_GPIO_INIT_H */
