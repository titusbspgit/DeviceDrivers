#include "tms320c6452_gpio_banked_architecture_32_signals_2_banks_drv.h"

static inline int bank_valid(uint8_t bank)
{
    return (bank == TMS320C6452_GPIO_BANK0) || (bank == TMS320C6452_GPIO_BANK1);
}

static inline uint32_t bank_shift(uint8_t bank)
{
    return (bank == TMS320C6452_GPIO_BANK1) ? 16u : 0u;
}

static tms320c6452_gpio_bank_isr_cb_t s_bank_cb[2] = { 0, 0 };
static void *s_bank_ctx[2] = { 0, 0 };

int tms320c6452_gpio_banked_arch_init(void)
{
    s_bank_cb[0] = 0; s_bank_cb[1] = 0;
    s_bank_ctx[0] = 0; s_bank_ctx[1] = 0;
    return TMS320C6452_GPIO_OK;
}

int tms320c6452_gpio_banked_arch_set_bank_dir(uint8_t bank, uint16_t dir_mask)
{
    if (!bank_valid(bank)) return TMS320C6452_GPIO_ERR_INVAL;

    const tms320c6452_gpio_core_ops_t *ops = tms320c6452_gpio_core_get_ops();
    uint32_t shift = bank_shift(bank);
    uint32_t mask32 = (uint32_t)0xFFFFu << shift;
    uint32_t dir32  = ((uint32_t)dir_mask & 0xFFFFu) << shift;

    uint32_t cur = ops->read32(TMS320C6452_GPIO_DIR01);
    cur = (cur & ~mask32) | dir32;
    ops->write32(TMS320C6452_GPIO_DIR01, cur);
    return TMS320C6452_GPIO_OK;
}

uint16_t tms320c6452_gpio_banked_arch_bank_read_dir(uint8_t bank)
{
    if (!bank_valid(bank)) return 0u;
    const tms320c6452_gpio_core_ops_t *ops = tms320c6452_gpio_core_get_ops();
    uint32_t shift = bank_shift(bank);
    uint32_t cur = ops->read32(TMS320C6452_GPIO_DIR01);
    return (uint16_t)((cur >> shift) & 0xFFFFu);
}

int tms320c6452_gpio_banked_arch_bank_set_bits(uint8_t bank, uint16_t set_mask)
{
    if (!bank_valid(bank)) return TMS320C6452_GPIO_ERR_INVAL;
    const tms320c6452_gpio_core_ops_t *ops = tms320c6452_gpio_core_get_ops();
    uint32_t shift = bank_shift(bank);
    uint32_t mask32 = ((uint32_t)set_mask & 0xFFFFu) << shift;
    ops->write32(TMS320C6452_GPIO_SET_DATA01, mask32);
    return TMS320C6452_GPIO_OK;
}

int tms320c6452_gpio_banked_arch_bank_clear_bits(uint8_t bank, uint16_t clr_mask)
{
    if (!bank_valid(bank)) return TMS320C6452_GPIO_ERR_INVAL;
    const tms320c6452_gpio_core_ops_t *ops = tms320c6452_gpio_core_get_ops();
    uint32_t shift = bank_shift(bank);
    uint32_t mask32 = ((uint32_t)clr_mask & 0xFFFFu) << shift;
    ops->write32(TMS320C6452_GPIO_CLR_DATA01, mask32);
    return TMS320C6452_GPIO_OK;
}

int tms320c6452_gpio_banked_arch_bank_write_out_mask(uint8_t bank, uint16_t value, uint16_t mask)
{
    if (!bank_valid(bank)) return TMS320C6452_GPIO_ERR_INVAL;
    const tms320c6452_gpio_core_ops_t *ops = tms320c6452_gpio_core_get_ops();

    uint32_t shift  = bank_shift(bank);
    uint32_t m32    = ((uint32_t)mask  & 0xFFFFu) << shift;
    uint32_t v32    = ((uint32_t)value & 0xFFFFu) << shift;

    uint32_t cur = ops->read32(TMS320C6452_GPIO_OUT_DATA01);
    cur = (cur & ~m32) | (v32 & m32);
    ops->write32(TMS320C6452_GPIO_OUT_DATA01, cur);
    return TMS320C6452_GPIO_OK;
}

uint16_t tms320c6452_gpio_banked_arch_bank_read_input(uint8_t bank)
{
    if (!bank_valid(bank)) return 0u;
    const tms320c6452_gpio_core_ops_t *ops = tms320c6452_gpio_core_get_ops();
    uint32_t shift = bank_shift(bank);
    uint32_t val = ops->read32(TMS320C6452_GPIO_IN_DATA01);
    return (uint16_t)((val >> shift) & 0xFFFFu);
}

uint16_t tms320c6452_gpio_banked_arch_bank_read_output(uint8_t bank)
{
    if (!bank_valid(bank)) return 0u;
    const tms320c6452_gpio_core_ops_t *ops = tms320c6452_gpio_core_get_ops();
    uint32_t shift = bank_shift(bank);
    uint32_t val = ops->read32(TMS320C6452_GPIO_OUT_DATA01);
    return (uint16_t)((val >> shift) & 0xFFFFu);
}

int tms320c6452_gpio_banked_arch_bank_enable_rising(uint8_t bank, uint16_t mask)
{
    if (!bank_valid(bank)) return TMS320C6452_GPIO_ERR_INVAL;
    const tms320c6452_gpio_core_ops_t *ops = tms320c6452_gpio_core_get_ops();
    uint32_t shift = bank_shift(bank);
    uint32_t mask32 = ((uint32_t)mask & 0xFFFFu) << shift;
    ops->write32(TMS320C6452_GPIO_SET_RIS_TRIG01, mask32);
    return TMS320C6452_GPIO_OK;
}

int tms320c6452_gpio_banked_arch_bank_disable_rising(uint8_t bank, uint16_t mask)
{
    if (!bank_valid(bank)) return TMS320C6452_GPIO_ERR_INVAL;
    const tms320c6452_gpio_core_ops_t *ops = tms320c6452_gpio_core_get_ops();
    uint32_t shift = bank_shift(bank);
    uint32_t mask32 = ((uint32_t)mask & 0xFFFFu) << shift;
    ops->write32(TMS320C6452_GPIO_CLR_RIS_TRIG01, mask32);
    return TMS320C6452_GPIO_OK;
}

int tms320c6452_gpio_banked_arch_bank_enable_falling(uint8_t bank, uint16_t mask)
{
    if (!bank_valid(bank)) return TMS320C6452_GPIO_ERR_INVAL;
    const tms320c6452_gpio_core_ops_t *ops = tms320c6452_gpio_core_get_ops();
    uint32_t shift = bank_shift(bank);
    uint32_t mask32 = ((uint32_t)mask & 0xFFFFu) << shift;
    ops->write32(TMS320C6452_GPIO_SET_FAL_TRIG01, mask32);
    return TMS320C6452_GPIO_OK;
}

int tms320c6452_gpio_banked_arch_bank_disable_falling(uint8_t bank, uint16_t mask)
{
    if (!bank_valid(bank)) return TMS320C6452_GPIO_ERR_INVAL;
    const tms320c6452_gpio_core_ops_t *ops = tms320c6452_gpio_core_get_ops();
    uint32_t shift = bank_shift(bank);
    uint32_t mask32 = ((uint32_t)mask & 0xFFFFu) << shift;
    ops->write32(TMS320C6452_GPIO_CLR_FAL_TRIG01, mask32);
    return TMS320C6452_GPIO_OK;
}

int tms320c6452_gpio_banked_arch_enable_bank_interrupt(uint8_t bank)
{
    if (!bank_valid(bank)) return TMS320C6452_GPIO_ERR_INVAL;
    const tms320c6452_gpio_core_ops_t *ops = tms320c6452_gpio_core_get_ops();
    uint32_t cur = ops->read32(TMS320C6452_GPIO_BINTEN);
    if (bank == TMS320C6452_GPIO_BANK0) cur |= TMS320C6452_GPIO_BINTEN_EN0;
    else                                cur |= TMS320C6452_GPIO_BINTEN_EN1;
    ops->write32(TMS320C6452_GPIO_BINTEN, cur);
    return TMS320C6452_GPIO_OK;
}

int tms320c6452_gpio_banked_arch_disable_bank_interrupt(uint8_t bank)
{
    if (!bank_valid(bank)) return TMS320C6452_GPIO_ERR_INVAL;
    const tms320c6452_gpio_core_ops_t *ops = tms320c6452_gpio_core_get_ops();
    uint32_t cur = ops->read32(TMS320C6452_GPIO_BINTEN);
    if (bank == TMS320C6452_GPIO_BANK0) cur &= ~TMS320C6452_GPIO_BINTEN_EN0;
    else                                cur &= ~TMS320C6452_GPIO_BINTEN_EN1;
    ops->write32(TMS320C6452_GPIO_BINTEN, cur);
    return TMS320C6452_GPIO_OK;
}

uint32_t tms320c6452_gpio_banked_arch_get_int_status(void)
{
    const tms320c6452_gpio_core_ops_t *ops = tms320c6452_gpio_core_get_ops();
    return ops->read32(TMS320C6452_GPIO_INTSTAT01);
}

int tms320c6452_gpio_banked_arch_register_isr(uint8_t bank, tms320c6452_gpio_bank_isr_cb_t cb, void *ctx)
{
    if (!bank_valid(bank)) return TMS320C6452_GPIO_ERR_INVAL;
    s_bank_cb[bank] = cb;
    s_bank_ctx[bank] = ctx;
    return TMS320C6452_GPIO_OK;
}

void tms320c6452_gpio_banked_arch_bank_isr(uint8_t bank)
{
    if (!bank_valid(bank)) return;
    const tms320c6452_gpio_core_ops_t *ops = tms320c6452_gpio_core_get_ops();
    uint32_t shift = bank_shift(bank);
    uint32_t isr = ops->read32(TMS320C6452_GPIO_INTSTAT01);
    uint16_t bank_status = (uint16_t)((isr >> shift) & 0xFFFFu);

    tms320c6452_gpio_bank_isr_cb_t cb = s_bank_cb[bank];
    void *ctx = s_bank_ctx[bank];

    if (cb && bank_status) {
        cb(bank_status, ctx);
        /* Clear handled pending bits (write 1 to clear) */
        uint32_t m32 = ((uint32_t)bank_status & 0xFFFFu) << shift;
        ops->write32(TMS320C6452_GPIO_INTSTAT01, m32);
    }
}

void tms320c6452_gpio_banked_arch_bank0_isr(void)
{
    tms320c6452_gpio_banked_arch_bank_isr(TMS320C6452_GPIO_BANK0);
}

void tms320c6452_gpio_banked_arch_bank1_isr(void)
{
    tms320c6452_gpio_banked_arch_bank_isr(TMS320C6452_GPIO_BANK1);
}

int tms320c6452_gpio_banked_arch_edge_ack(uint8_t bank, uint16_t mask)
{
    if (!bank_valid(bank)) return TMS320C6452_GPIO_ERR_INVAL;
    const tms320c6452_gpio_core_ops_t *ops = tms320c6452_gpio_core_get_ops();
    uint32_t shift  = bank_shift(bank);
    uint32_t m32    = ((uint32_t)mask & 0xFFFFu) << shift;

    /* Disable both edges for mask to avoid retrigger during servicing */
    ops->write32(TMS320C6452_GPIO_CLR_RIS_TRIG01, m32);
    ops->write32(TMS320C6452_GPIO_CLR_FAL_TRIG01, m32);
    ops->barrier();

    /* Clear pending interrupts (W1C) for the selected pins */
    ops->write32(TMS320C6452_GPIO_INTSTAT01, m32);
    ops->barrier();

    /* Re-enable both edges for mask (deterministic re-arm) */
    ops->write32(TMS320C6452_GPIO_SET_RIS_TRIG01, m32);
    ops->write32(TMS320C6452_GPIO_SET_FAL_TRIG01, m32);
    return TMS320C6452_GPIO_OK;
}
