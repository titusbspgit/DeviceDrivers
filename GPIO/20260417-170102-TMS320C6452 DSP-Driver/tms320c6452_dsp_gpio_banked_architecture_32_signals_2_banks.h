#ifndef TMS320C6452_DSP_GPIO_BANKED_ARCHITECTURE_32_SIGNALS_2_BANKS_H
#define TMS320C6452_DSP_GPIO_BANKED_ARCHITECTURE_32_SIGNALS_2_BANKS_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Controller configuration */
#define TMS320C6452_DSP_GPIO_BASE      ((uintptr_t)0xA1008000u)

/* Register Offsets (relative to GPIO base) */
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

/* Bit helpers */
#define GPIO_BIT(n)                   ((uint32_t)1u << (uint32_t)(n))
#define GPIO_BANK0_MASK               ((uint32_t)0x0000FFFFu)
#define GPIO_BANK1_MASK               ((uint32_t)0xFFFF0000u)

/* API: Banked architecture control for 32 GPIO signals across 2 banks */
void     tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_init(void);

/* Set direction mask for all 32 pins at once: bit=1 -> input, bit=0 -> output */
bool     tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_set_dir_mask(uint32_t input_mask);

/* Atomic output control using SET/CLEAR masks (affects only pins configured as outputs) */
bool     tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_write_mask(uint32_t set_mask, uint32_t clear_mask);

/* Read input and output latches per bank (16-bit each). bank=0 or 1. */
uint16_t tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_read_in_bank(unsigned bank);
uint16_t tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_read_out_bank(unsigned bank);

/* Enable/disable bank-level interrupt (BINTEN) for bank 0/1 */
bool     tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_enable_bank_interrupt(unsigned bank, bool enable);

/* Configure per-pin edge detection (rising/falling). pin in [0..31] */
bool     tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_configure_pin_edge(unsigned pin, bool rise_en, bool fall_en);

/* Interrupt status helpers (W1C) */
uint32_t tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_get_int_status(void);
void     tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks_clear_int_status(uint32_t mask);

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_DSP_GPIO_BANKED_ARCHITECTURE_32_SIGNALS_2_BANKS_H */
