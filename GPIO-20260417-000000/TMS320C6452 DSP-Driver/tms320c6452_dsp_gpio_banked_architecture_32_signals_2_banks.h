/*
 * TMS320C6452 DSP - GPIO banked architecture (32 signals, 2 banks) - Feature Driver (Header)
 * Constraints: Memory-mapped I/O, No dynamic allocation, Deterministic register access
 */
#ifndef TMS320C6452_DSP_GPIO_BANKED_ARCHITECTURE_32_SIGNALS_2_BANKS_H
#define TMS320C6452_DSP_GPIO_BANKED_ARCHITECTURE_32_SIGNALS_2_BANKS_H

#include <stdint.h>
#include "TMS320C6452 DSP_core_drv.h"

#ifdef __cplusplus
extern "C" {
#endif

/* GPIO module base (from device reference) */
#define TMS320C6452_DSP_GPIO_BASE      (0x02B00000u)

/* Compute offsets relative to controller base for core ops */
#define TMS320C6452_DSP_GPIO_OFF(x)    ((u32)((TMS320C6452_DSP_GPIO_BASE - TMS320C6452_DSP_BASE) + (u32)(x)))

/* Register offsets (from GPIO base) */
#define GPIO_PID_OFF                    (0x00u)
#define GPIO_BINTEN_OFF                 (0x08u)
#define GPIO_DIR01_OFF                  (0x10u)
#define GPIO_OUT_DATA01_OFF             (0x14u)
#define GPIO_SET_DATA01_OFF             (0x18u)
#define GPIO_CLR_DATA01_OFF             (0x1Cu)
#define GPIO_IN_DATA01_OFF              (0x20u)
#define GPIO_SET_RIS_TRIG01_OFF         (0x24u)
#define GPIO_CLR_RIS_TRIG01_OFF         (0x28u)
#define GPIO_SET_FAL_TRIG01_OFF         (0x2Cu)
#define GPIO_CLR_FAL_TRIG01_OFF         (0x30u)
#define GPIO_INTSTAT01_OFF              (0x34u)

/* Bank helpers */
#define GPIO_BANKS                      (2u)
#define GPIO_PINS_PER_BANK              (16u)
#define GPIO_BANK_SHIFT(b)              ((u32)((b) * GPIO_PINS_PER_BANK))
#define GPIO_BANK_MASK(b, m16)          ((u32)(m16) << GPIO_BANK_SHIFT(b))
#define GPIO_U32_TO_BANK16(b, v32)      ((u16)(((u32)(v32) >> GPIO_BANK_SHIFT(b)) & 0xFFFFu))

/* Public API: Single-feature (banked architecture utilities) */
int  tms320c6452_dsp_gpio_arch_init(void);
void tms320c6452_dsp_gpio_set_dir_mask(u32 bank, u16 mask16, u8 dir_input);
void tms320c6452_dsp_gpio_write_bank_mask(u32 bank, u16 set_mask16, u16 clr_mask16);
u16  tms320c6452_dsp_gpio_read_inputs_bank(u32 bank);
u16  tms320c6452_dsp_gpio_read_outputs_bank(u32 bank);
void tms320c6452_dsp_gpio_bank_irq_enable(u32 bank, u8 enable);
void tms320c6452_dsp_gpio_config_edge_bank(u32 bank, u16 rise_en_mask16, u16 fall_en_mask16);
u16  tms320c6452_dsp_gpio_read_and_clear_int_bank(u32 bank);

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_DSP_GPIO_BANKED_ARCHITECTURE_32_SIGNALS_2_BANKS_H */
