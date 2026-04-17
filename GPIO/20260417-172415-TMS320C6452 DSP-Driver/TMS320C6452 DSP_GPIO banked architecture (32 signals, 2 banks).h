#ifndef TMS320C6452_DSP_GPIO_BANKED_ARCH_32_2_H
#define TMS320C6452_DSP_GPIO_BANKED_ARCH_32_2_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TMS320C6452_DSP_GPIO_BASE      ((uintptr_t)0xA1008000u)

#define GPIO_PID_OFFSET               (0x00u)
#define GPIO_BINTEN_OFFSET            (0x08u)
#define GPIO_DIR01_OFFSET             (0x10u)
#define GPIO_OUT_DATA01_OFFSET        (0x14u)
#define GPIO_SET_DATA01_OFFSET        (0x18u)
#define GPIO_CLR_DATA01_OFFSET        (0x1Cu)
#define GPIO_IN_DATA01_OFFSET         (0x20u)
#define GPIO_SET_RIS_TRIG01_OFFSET    (0x24u)
#define GPIO_CLR_RIS_TRIG01_OFFSET    (0x28u)
#define GPIO_SET_FAL_TRIG01_OFFSET    (0x2Cu)
#define GPIO_CLR_FAL_TRIG01_OFFSET    (0x30u)
#define GPIO_INTSTAT01_OFFSET         (0x34u)

#define GPIO_BIT(n)                   ((uint32_t)1u << (uint32_t)(n))

void     TMS320C6452_DSP_GPIO_banked_architecture_32_signals_2_banks_init(void);
bool     TMS320C6452_DSP_GPIO_banked_architecture_32_signals_2_banks_set_dir_mask(uint32_t input_mask);
bool     TMS320C6452_DSP_GPIO_banked_architecture_32_signals_2_banks_write_mask(uint32_t set_mask, uint32_t clear_mask);
uint16_t TMS320C6452_DSP_GPIO_banked_architecture_32_signals_2_banks_read_in_bank(unsigned bank);
uint16_t TMS320C6452_DSP_GPIO_banked_architecture_32_signals_2_banks_read_out_bank(unsigned bank);
bool     TMS320C6452_DSP_GPIO_banked_architecture_32_signals_2_banks_enable_bank_interrupt(unsigned bank, bool enable);
bool     TMS320C6452_DSP_GPIO_banked_architecture_32_signals_2_banks_configure_pin_edge(unsigned pin, bool rise_en, bool fall_en);
uint32_t TMS320C6452_DSP_GPIO_banked_architecture_32_signals_2_banks_get_int_status(void);
void     TMS320C6452_DSP_GPIO_banked_architecture_32_signals_2_banks_clear_int_status(uint32_t mask);

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_DSP_GPIO_BANKED_ARCH_32_2_H */
