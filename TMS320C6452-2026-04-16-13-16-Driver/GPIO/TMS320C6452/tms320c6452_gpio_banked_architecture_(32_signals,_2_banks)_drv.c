#include "tms320c6452_gpio_banked_architecture_(32_signals,_2_banks).h"

/* Static context for this single feature */
static const tms320c6452_gpio_core_api_t* s_core;
static tms320c6452_gpio_cb_t s_callbacks[32];
static void* s_cb_ctx[32];

static inline int pin_valid(uint32_t pin)
{
    return (pin < 32u) ? 1 : 0;
}

int tms320c6452_gpio_banked_arch_init(const tms320c6452_gpio_core_api_t* core)
{
    if (!core) return TMS320C6452_GPIO_EINVAL;
    s_core = core;
    for (unsigned i = 0; i < 32u; ++i) { s_callbacks[i] = 0; s_cb_ctx[i] = 0; }
    /* Dependencies (pinmux, LPSC clock enable) are platform-specific: UNKNOWN */
    /* All pins reset as inputs; no further init needed here. */
    (void)core;
    return TMS320C6452_GPIO_OK;
}

int tms320c6452_gpio_set_outputs(uint32_t mask)
{
    if (!s_core) return TMS320C6452_GPIO_EINVAL;
    /* DIR01: 0=output */
    s_core->clear_bits(GPIO_DIR01_OFFSET, mask);
    return TMS320C6452_GPIO_OK;
}

int tms320c6452_gpio_set_inputs(uint32_t mask)
{
    if (!s_core) return TMS320C6452_GPIO_EINVAL;
    /* DIR01: 1=input */
    s_core->set_bits(GPIO_DIR01_OFFSET, mask);
    return TMS320C6452_GPIO_OK;
}

int tms320c6452_gpio_set_pins(uint32_t mask)
{
    if (!s_core) return TMS320C6452_GPIO_EINVAL;
    s_core->write_reg(GPIO_SET_DATA01_OFFSET, mask);
    return TMS320C6452_GPIO_OK;
}

int tms320c6452_gpio_clear_pins(uint32_t mask)
{
    if (!s_core) return TMS320C6452_GPIO_EINVAL;
    s_core->write_reg(GPIO_CLR_DATA01_OFFSET, mask);
    return TMS320C6452_GPIO_OK;
}

uint32_t tms320c6452_gpio_read_inputs(void)
{
    if (!s_core) return 0u;
    return s_core->read_reg(GPIO_IN_DATA01_OFFSET);
}

uint32_t tms320c6452_gpio_read_outputs(void)
{
    if (!s_core) return 0u;
    return s_core->read_reg(GPIO_OUT_DATA01_OFFSET);
}

int tms320c6452_gpio_enable_rising(uint32_t mask)
{
    if (!s_core) return TMS320C6452_GPIO_EINVAL;
    s_core->write_reg(GPIO_SET_RIS_TRIG01_OFFSET, mask);
    return TMS320C6452_GPIO_OK;
}

int tms320c6452_gpio_disable_rising(uint32_t mask)
{
    if (!s_core) return TMS320C6452_GPIO_EINVAL;
    s_core->write_reg(GPIO_CLR_RIS_TRIG01_OFFSET, mask);
    return TMS320C6452_GPIO_OK;
}

int tms320c6452_gpio_enable_falling(uint32_t mask)
{
    if (!s_core) return TMS320C6452_GPIO_EINVAL;
    s_core->write_reg(GPIO_SET_FAL_TRIG01_OFFSET, mask);
    return TMS320C6452_GPIO_OK;
}

int tms320c6452_gpio_disable_falling(uint32_t mask)
{
    if (!s_core) return TMS320C6452_GPIO_EINVAL;
    s_core->write_reg(GPIO_CLR_FAL_TRIG01_OFFSET, mask);
    return TMS320C6452_GPIO_OK;
}

int tms320c6452_gpio_enable_bank_interrupts(uint32_t bank_mask)
{
    if (!s_core) return TMS320C6452_GPIO_EINVAL;
    uint32_t v = s_core->read_reg(GPIO_BINTEN_OFFSET);
    v |= (bank_mask & (GPIO_BINTEN_EN0 | GPIO_BINTEN_EN1));
    s_core->write_reg(GPIO_BINTEN_OFFSET, v);
    return TMS320C6452_GPIO_OK;
}

uint32_t tms320c6452_gpio_read_int_status(void)
{
    if (!s_core) return 0u;
    return s_core->read_reg(GPIO_INTSTAT01_OFFSET);
}

int tms320c6452_gpio_register_callback(uint32_t pin, tms320c6452_gpio_cb_t cb, void* ctx)
{
    if (!s_core) return TMS320C6452_GPIO_EINVAL;
    if (!pin_valid(pin)) return TMS320C6452_GPIO_EINVAL;
    s_callbacks[pin] = cb;
    s_cb_ctx[pin] = ctx;
    return TMS320C6452_GPIO_OK;
}

void tms320c6452_gpio_isr(void)
{
    if (!s_core) return;
    uint32_t status = s_core->read_reg(GPIO_INTSTAT01_OFFSET);
    while (status) {
        uint32_t bit = status & (~status + 1u); /* isolate lowest set bit */
        uint32_t pin = __builtin_ctz(status);
        if (pin < 32u && s_callbacks[pin]) {
            s_callbacks[pin](pin, s_cb_ctx[pin]);
        }
        status &= ~bit;
    }
    /* CPU/SoC-specific interrupt acknowledge: UNKNOWN */
}
