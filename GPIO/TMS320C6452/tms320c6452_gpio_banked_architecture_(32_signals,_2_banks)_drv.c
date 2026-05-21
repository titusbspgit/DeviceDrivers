#include "tms320c6452_gpio_banked_architecture_(32_signals,_2_banks).h"

/* Internal helpers */
static int _valid_handle(const tms320c6452_gpio_banked_architecture_32_signals_2_banks_handle_t *h)
{
    return (h != 0) && (h->core != 0) && (h->core->read32 != 0) && (h->core->write32 != 0);
}

static int _valid_gpio(uint32_t gpio)
{
    return (gpio < 32u);
}

static int _valid_bank(uint32_t bank)
{
    return (bank < 2u);
}

int tms320c6452_gpio_banked_architecture_32_signals_2_banks_init(
    tms320c6452_gpio_banked_architecture_32_signals_2_banks_handle_t *h,
    const tms320c6452_gpio_core_t *core)
{
    if (!h || !core) return TMS_GPIO_ERR_INVAL;
    h->core = core;

    /* Disable all bank interrupts */
    uint32_t binten = 0u;
    h->core->write32(h->core->base, GPIO_BINTEN_OFFSET, binten);

    /* Disable all per-pin triggers */
    h->core->write32(h->core->base, GPIO_CLR_RIS_TRIG01_OFFSET, 0xFFFFFFFFu);
    h->core->write32(h->core->base, GPIO_CLR_FAL_TRIG01_OFFSET, 0xFFFFFFFFu);

    /* Clear any pending status */
    uint32_t pending = h->core->read32(h->core->base, GPIO_INTSTAT01_OFFSET);
    if (pending)
        h->core->write32(h->core->base, GPIO_INTSTAT01_OFFSET, pending);

    return TMS_GPIO_OK;
}

int tms320c6452_gpio_banked_architecture_32_signals_2_banks_set_direction(
    tms320c6452_gpio_banked_architecture_32_signals_2_banks_handle_t *h,
    uint32_t gpio, int is_input)
{
    if (!_valid_handle(h)) return TMS_GPIO_ERR_INVAL;
    if (!_valid_gpio(gpio)) return TMS_GPIO_ERR_RANGE;

    uint32_t dir = h->core->read32(h->core->base, GPIO_DIR01_OFFSET);
    if (is_input)
        dir |= GPIO_BIT(gpio);  /* 1 = input */
    else
        dir &= ~GPIO_BIT(gpio); /* 0 = output */
    h->core->write32(h->core->base, GPIO_DIR01_OFFSET, dir);
    return TMS_GPIO_OK;
}

int tms320c6452_gpio_banked_architecture_32_signals_2_banks_write_pin(
    tms320c6452_gpio_banked_architecture_32_signals_2_banks_handle_t *h,
    uint32_t gpio, int value)
{
    if (!_valid_handle(h)) return TMS_GPIO_ERR_INVAL;
    if (!_valid_gpio(gpio)) return TMS_GPIO_ERR_RANGE;

    /* Drive only if configured as output */
    uint32_t dir = h->core->read32(h->core->base, GPIO_DIR01_OFFSET);
    if ( (dir & GPIO_BIT(gpio)) != 0u ) {
        /* configured as input -> ignore */
        return TMS_GPIO_ERR_INVAL;
    }

    if (value)
        h->core->write32(h->core->base, GPIO_SET_DATA01_OFFSET, GPIO_BIT(gpio));
    else
        h->core->write32(h->core->base, GPIO_CLR_DATA01_OFFSET, GPIO_BIT(gpio));

    return TMS_GPIO_OK;
}

int tms320c6452_gpio_banked_architecture_32_signals_2_banks_read_pin(
    tms320c6452_gpio_banked_architecture_32_signals_2_banks_handle_t *h,
    uint32_t gpio, int *value)
{
    if (!_valid_handle(h) || !value) return TMS_GPIO_ERR_INVAL;
    if (!_valid_gpio(gpio)) return TMS_GPIO_ERR_RANGE;

    uint32_t v = h->core->read32(h->core->base, GPIO_IN_DATA01_OFFSET);
    *value = ( (v & GPIO_BIT(gpio)) != 0u ) ? 1 : 0;
    return TMS_GPIO_OK;
}

int tms320c6452_gpio_banked_architecture_32_signals_2_banks_enable_bank_interrupt(
    tms320c6452_gpio_banked_architecture_32_signals_2_banks_handle_t *h,
    uint32_t bank, int enable)
{
    if (!_valid_handle(h)) return TMS_GPIO_ERR_INVAL;
    if (!_valid_bank(bank)) return TMS_GPIO_ERR_RANGE;

    uint32_t binten = h->core->read32(h->core->base, GPIO_BINTEN_OFFSET);
    uint32_t mask = (uint32_t)1u << bank; /* bit0->bank0, bit1->bank1 */
    if (enable)
        binten |= mask;
    else
        binten &= ~mask;
    h->core->write32(h->core->base, GPIO_BINTEN_OFFSET, binten);
    return TMS_GPIO_OK;
}

int tms320c6452_gpio_banked_architecture_32_signals_2_banks_configure_pin_interrupt(
    tms320c6452_gpio_banked_architecture_32_signals_2_banks_handle_t *h,
    uint32_t gpio, int rising, int falling)
{
    if (!_valid_handle(h)) return TMS_GPIO_ERR_INVAL;
    if (!_valid_gpio(gpio)) return TMS_GPIO_ERR_RANGE;

    uint32_t bit = GPIO_BIT(gpio);

    /* Disable first */
    h->core->write32(h->core->base, GPIO_CLR_RIS_TRIG01_OFFSET, bit);
    h->core->write32(h->core->base, GPIO_CLR_FAL_TRIG01_OFFSET, bit);

    /* Clear any pending latched status on this pin */
    uint32_t st = h->core->read32(h->core->base, GPIO_INTSTAT01_OFFSET);
    if (st & bit)
        h->core->write32(h->core->base, GPIO_INTSTAT01_OFFSET, bit);

    if (rising)
        h->core->write32(h->core->base, GPIO_SET_RIS_TRIG01_OFFSET, bit);
    if (falling)
        h->core->write32(h->core->base, GPIO_SET_FAL_TRIG01_OFFSET, bit);

    return TMS_GPIO_OK;
}

uint32_t tms320c6452_gpio_banked_architecture_32_signals_2_banks_get_int_status(
    tms320c6452_gpio_banked_architecture_32_signals_2_banks_handle_t *h)
{
    if (!_valid_handle(h)) return 0u;
    return h->core->read32(h->core->base, GPIO_INTSTAT01_OFFSET);
}

void tms320c6452_gpio_banked_architecture_32_signals_2_banks_clear_int_status(
    tms320c6452_gpio_banked_architecture_32_signals_2_banks_handle_t *h,
    uint32_t mask)
{
    if (!_valid_handle(h)) return;
    h->core->write32(h->core->base, GPIO_INTSTAT01_OFFSET, mask);
}
