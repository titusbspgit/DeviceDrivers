/*
 * TMS320C6452 DSP - GPIO banked architecture (32 signals, 2 banks) - Feature Test
 * Sequence: init -> basic DIR/out tests on bank0 -> edge config -> pending/clear verify
 */
#include <stdint.h>
#include "tms320c6452_dsp_gpio_banked_architecture_32_signals_2_banks.h"

static int expect_eq_u32(uint32_t a, uint32_t b) { return (a == b) ? 0 : -1; }
static int expect_eq_u16(uint16_t a, uint16_t b) { return (a == b) ? 0 : -1; }

int main(void)
{
    int rc;
    uint32_t v32;
    uint16_t v16;

    rc = tms320c6452_dsp_gpio_arch_init();
    if (rc != 0) return rc;

    /* Verify post-init state on key registers (deterministic) */
    v32 = tms320c6452_core_get_ops()->read32(TMS320C6452_DSP_GPIO_OFF(GPIO_BINTEN_OFF));
    if (expect_eq_u32(v32, 0x00000000u)) return -10;

    v32 = tms320c6452_core_get_ops()->read32(TMS320C6452_DSP_GPIO_OFF(GPIO_DIR01_OFF));
    if (expect_eq_u32(v32, 0xFFFFFFFFu)) return -11;

    v32 = tms320c6452_core_get_ops()->read32(TMS320C6452_DSP_GPIO_OFF(GPIO_SET_RIS_TRIG01_OFF));
    if (expect_eq_u32(v32, 0x00000000u)) return -12;

    v32 = tms320c6452_core_get_ops()->read32(TMS320C6452_DSP_GPIO_OFF(GPIO_SET_FAL_TRIG01_OFF));
    if (expect_eq_u32(v32, 0x00000000u)) return -13;

    v32 = tms320c6452_core_get_ops()->read32(TMS320C6452_DSP_GPIO_OFF(GPIO_INTSTAT01_OFF));
    if (expect_eq_u32(v32, 0x00000000u)) return -14;

    /* Configure some pins on bank0 as outputs and toggle them */
    tms320c6452_dsp_gpio_set_dir_mask(0u, (uint16_t)0x000Fu, 0u); /* GP[3:0] -> outputs */
    v32 = tms320c6452_core_get_ops()->read32(TMS320C6452_DSP_GPIO_OFF(GPIO_DIR01_OFF));
    if (expect_eq_u32(v32 & 0x0000FFFFu, 0x0000FFF0u)) return -20; /* lower 4 cleared */

    tms320c6452_dsp_gpio_write_bank_mask(0u, (uint16_t)0x0005u, (uint16_t)0x000Au); /* set 0,2; clear 1,3 */
    v16 = tms320c6452_dsp_gpio_read_outputs_bank(0u);
    if (expect_eq_u16((uint16_t)(v16 & 0x000Fu), (uint16_t)0x0005u)) return -21;

    /* Edge configuration on bank0: enable rising on bit4, falling on bit5 (both inputs by default) */
    tms320c6452_dsp_gpio_config_edge_bank(0u, (uint16_t)0x0010u, (uint16_t)0x0020u);

    /* Enable bank0 interrupt export */
    tms320c6452_dsp_gpio_bank_irq_enable(0u, 1u);
    v32 = tms320c6452_core_get_ops()->read32(TMS320C6452_DSP_GPIO_OFF(GPIO_BINTEN_OFF));
    if (expect_eq_u32(v32 & 0x00000003u, 0x00000001u)) return -30;

    /* Clear any pending (deterministic clear-all) */
    (void)tms320c6452_dsp_gpio_read_and_clear_int_bank(0u);

    /* Final deterministic clear: write W1C on all */
    tms320c6452_core_get_ops()->write32(TMS320C6452_DSP_GPIO_OFF(GPIO_INTSTAT01_OFF), 0xFFFFFFFFu);
    v32 = tms320c6452_core_get_ops()->read32(TMS320C6452_DSP_GPIO_OFF(GPIO_INTSTAT01_OFF));
    if (expect_eq_u32(v32, 0x00000000u)) return -40;

    return 0;
}
