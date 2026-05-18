#include "tms320c6452_dsp_gpio_banked_architecture__32_signals__2_banks.h"

static volatile int g_test_status = 0;

static void expect_eq_i(int a, int b, int line)
{
    if (a != b)
    {
        g_test_status = line;
    }
}

int main(void)
{
    tms320c6452_dsp_gpio_banked_ctx_t ctx;
    tms320c6452_dsp_gpio_banked_cfg_t cfg;

    cfg.base          = TMS320C6452_DSP_GPIO_BASE_ADDR;
    cfg.bank_stride   = TMS320C6452_DSP_GPIO_REG_UNKNOWN;
    cfg.dir_offset    = TMS320C6452_DSP_GPIO_REG_UNKNOWN;
    cfg.in_offset     = TMS320C6452_DSP_GPIO_REG_UNKNOWN;
    cfg.out_offset    = TMS320C6452_DSP_GPIO_REG_UNKNOWN;
    cfg.set_offset    = TMS320C6452_DSP_GPIO_REG_UNKNOWN;
    cfg.clr_offset    = TMS320C6452_DSP_GPIO_REG_UNKNOWN;
    cfg.int_en_offset = TMS320C6452_DSP_GPIO_REG_UNKNOWN;
    cfg.int_sts_offset= TMS320C6452_DSP_GPIO_REG_UNKNOWN;
    cfg.int_clr_offset= TMS320C6452_DSP_GPIO_REG_UNKNOWN;

    expect_eq_i(tms320c6452_dsp_gpio_banked_init(&ctx, &cfg), TMS320C6452_DSP_GPIO_OK, __LINE__);
    expect_eq_i(tms320c6452_dsp_gpio_banked_init(NULL, &cfg), TMS320C6452_DSP_GPIO_ERR_PARAM, __LINE__);
    expect_eq_i(tms320c6452_dsp_gpio_banked_init(&ctx, NULL), TMS320C6452_DSP_GPIO_ERR_PARAM, __LINE__);

    expect_eq_i(tms320c6452_dsp_gpio_banked_config_dir_bank(&ctx, TMS320C6452_DSP_GPIO_BANK_COUNT, 0xFFFFu, true),
                TMS320C6452_DSP_GPIO_ERR_PARAM, __LINE__);
    expect_eq_i(tms320c6452_dsp_gpio_banked_write_bank_mask(&ctx, TMS320C6452_DSP_GPIO_BANK_COUNT, 0xFFFFu, 0u),
                TMS320C6452_DSP_GPIO_ERR_PARAM, __LINE__);
    {
        uint32_t mask_dummy = 0u;
        expect_eq_i(tms320c6452_dsp_gpio_banked_read_bank(&ctx, TMS320C6452_DSP_GPIO_BANK_COUNT, &mask_dummy),
                    TMS320C6452_DSP_GPIO_ERR_PARAM, __LINE__);
    }
    expect_eq_i(tms320c6452_dsp_gpio_banked_write_pin(&ctx, TMS320C6452_DSP_GPIO_TOTAL_PINS, true),
                TMS320C6452_DSP_GPIO_ERR_PARAM, __LINE__);
    {
        bool v;
        expect_eq_i(tms320c6452_dsp_gpio_banked_read_pin(&ctx, TMS320C6452_DSP_GPIO_TOTAL_PINS, &v),
                    TMS320C6452_DSP_GPIO_ERR_PARAM, __LINE__);
        expect_eq_i(tms320c6452_dsp_gpio_banked_read_pin(&ctx, 0u, NULL),
                    TMS320C6452_DSP_GPIO_ERR_PARAM, __LINE__);
    }

    expect_eq_i(tms320c6452_dsp_gpio_banked_config_dir_bank(&ctx, 0u, 0x0000FFFFu, true),
                TMS320C6452_DSP_GPIO_ERR_UNSUPPORTED, __LINE__);
    expect_eq_i(tms320c6452_dsp_gpio_banked_write_bank_mask(&ctx, 0u, 0x00000001u, 0u),
                TMS320C6452_DSP_GPIO_ERR_UNSUPPORTED, __LINE__);
    {
        uint32_t mask = 0u;
        expect_eq_i(tms320c6452_dsp_gpio_banked_read_bank(&ctx, 0u, &mask),
                    TMS320C6452_DSP_GPIO_ERR_UNSUPPORTED, __LINE__);
    }
    expect_eq_i(tms320c6452_dsp_gpio_banked_write_pin(&ctx, 0u, true),
                TMS320C6452_DSP_GPIO_ERR_UNSUPPORTED, __LINE__);
    {
        bool v = false;
        expect_eq_i(tms320c6452_dsp_gpio_banked_read_pin(&ctx, 0u, &v),
                    TMS320C6452_DSP_GPIO_ERR_UNSUPPORTED, __LINE__);
    }
    expect_eq_i(tms320c6452_dsp_gpio_banked_irq_enable_bank(&ctx, 0u, 0x0000FFFFu),
                TMS320C6452_DSP_GPIO_ERR_UNSUPPORTED, __LINE__);
    expect_eq_i(tms320c6452_dsp_gpio_banked_irq_clear_bank(&ctx, 0u, 0x0000FFFFu),
                TMS320C6452_DSP_GPIO_ERR_UNSUPPORTED, __LINE__);

    {
        uint32_t p0_bank  = TMS320C6452_DSP_GPIO_PIN_TO_BANK(0u);
        uint32_t p0_bit   = TMS320C6452_DSP_GPIO_PIN_TO_BIT(0u);
        uint32_t p15_bank = TMS320C6452_DSP_GPIO_PIN_TO_BANK(15u);
        uint32_t p15_bit  = TMS320C6452_DSP_GPIO_PIN_TO_BIT(15u);
        uint32_t p16_bank = TMS320C6452_DSP_GPIO_PIN_TO_BANK(16u);
        uint32_t p16_bit  = TMS320C6452_DSP_GPIO_PIN_TO_BIT(16u);
        uint32_t p31_bank = TMS320C6452_DSP_GPIO_PIN_TO_BANK(31u);
        uint32_t p31_bit  = TMS320C6452_DSP_GPIO_PIN_TO_BIT(31u);
        (void)p0_bank; (void)p0_bit; (void)p15_bank; (void)p15_bit;
        (void)p16_bank; (void)p16_bit; (void)p31_bank; (void)p31_bit;
    }

    return g_test_status;
}
