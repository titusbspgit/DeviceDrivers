/*
 * TMS320C6452 DSP - GPIO banked architecture (32 signals, 2 banks) - Feature Driver (Source)
 * Implements banked operations using shared core ops for MMIO.
 */
#include <stdint.h>
#include "tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks.h"

static inline const tms320c6452_core_ops_t *ops(void)
{
    return tms320c6452_core_get_ops();
}

static inline void wr32(u32 reg_off, u32 v)
{
    ops()->write32(TMS320C6452_DSP_GPIO_OFF(reg_off), v);
}

static inline u32 rd32(u32 reg_off)
{
    return ops()->read32(TMS320C6452_DSP_GPIO_OFF(reg_off));
}

static inline u32 bank_sanitized(u32 bank) { return (bank < GPIO_BANKS) ? bank : 0u; }

int tms320c6452_dsp_gpio_arch_init(void)
{
    /* Disable bank-level interrupt exports */
    wr32(GPIO_BINTEN_OFF, 0x00000000u);

    /* Disable all rising/falling edge detection for both banks */
    wr32(GPIO_CLR_RIS_TRIG01_OFF, 0xFFFFFFFFu);
    wr32(GPIO_CLR_FAL_TRIG01_OFF, 0xFFFFFFFFu);

    /* Clear any pending interrupt status (W1C) */
    wr32(GPIO_INTSTAT01_OFF, 0xFFFFFFFFu);

    /* Program all GPIOs as inputs (safe default) */
    wr32(GPIO_DIR01_OFF, 0xFFFFFFFFu);

    /* Deterministic verification */
    if (rd32(GPIO_BINTEN_OFF) != 0x00000000u) return -1;
    if (rd32(GPIO_DIR01_OFF)  != 0xFFFFFFFFu) return -2;
    if (rd32(GPIO_SET_RIS_TRIG01_OFF) != 0x00000000u) return -3;
    if (rd32(GPIO_SET_FAL_TRIG01_OFF) != 0x00000000u) return -4;
    if (rd32(GPIO_INTSTAT01_OFF) != 0x00000000u) return -5;

    return 0;
}

void tms320c6452_dsp_gpio_set_dir_mask(u32 bank_in, u16 mask16, u8 dir_input)
{
    u32 bank = bank_sanitized(bank_in);
    u32 cur = rd32(GPIO_DIR01_OFF);
    u32 m = GPIO_BANK_MASK(bank, mask16);

    if (dir_input) cur |= m;   /* 1 -> input */
    else           cur &= ~m;  /* 0 -> output */

    wr32(GPIO_DIR01_OFF, cur);
}

void tms320c6452_dsp_gpio_write_bank_mask(u32 bank_in, u16 set_mask16, u16 clr_mask16)
{
    u32 bank = bank_sanitized(bank_in);
    u32 setm = GPIO_BANK_MASK(bank, set_mask16);
    u32 clrm = GPIO_BANK_MASK(bank, clr_mask16);

    if (setm) wr32(GPIO_SET_DATA01_OFF, setm);
    if (clrm) wr32(GPIO_CLR_DATA01_OFF, clrm);
}

u16 tms320c6452_dsp_gpio_read_inputs_bank(u32 bank_in)
{
    u32 bank = bank_sanitized(bank_in);
    u32 v = rd32(GPIO_IN_DATA01_OFF);
    return GPIO_U32_TO_BANK16(bank, v);
}

u16 tms320c6452_dsp_gpio_read_outputs_bank(u32 bank_in)
{
    u32 bank = bank_sanitized(bank_in);
    u32 v = rd32(GPIO_OUT_DATA01_OFF);
    return GPIO_U32_TO_BANK16(bank, v);
}

void tms320c6452_dsp_gpio_bank_irq_enable(u32 bank_in, u8 enable)
{
    u32 bank = bank_sanitized(bank_in);
    u32 v = rd32(GPIO_BINTEN_OFF);
    u32 bit = (bank == 0u) ? (1u << 0) : (1u << 1);

    if (enable) v |= bit; else v &= ~bit;
    wr32(GPIO_BINTEN_OFF, v);
}

void tms320c6452_dsp_gpio_config_edge_bank(u32 bank_in, u16 rise_en_mask16, u16 fall_en_mask16)
{
    u32 bank = bank_sanitized(bank_in);
    u32 bank_mask = GPIO_BANK_MASK(bank, 0xFFFFu);

    /* Clear all then set requested masks to get an exact map */
    wr32(GPIO_CLR_RIS_TRIG01_OFF, bank_mask);
    wr32(GPIO_SET_RIS_TRIG01_OFF, GPIO_BANK_MASK(bank, rise_en_mask16));

    wr32(GPIO_CLR_FAL_TRIG01_OFF, bank_mask);
    wr32(GPIO_SET_FAL_TRIG01_OFF, GPIO_BANK_MASK(bank, fall_en_mask16));
}

u16 tms320c6452_dsp_gpio_read_and_clear_int_bank(u32 bank_in)
{
    u32 bank = bank_sanitized(bank_in);
    u32 v = rd32(GPIO_INTSTAT01_OFF);
    u32 m = GPIO_BANK_MASK(bank, 0xFFFFu);
    u16 ret = GPIO_U32_TO_BANK16(bank, v);
    if (ret) {
        wr32(GPIO_INTSTAT01_OFF, (v & m)); /* W1C only bank bits that were set */
    }
    return ret;
}
