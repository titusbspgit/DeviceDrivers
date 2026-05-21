#include "tms320c6452_gpio_pin_multiplexing_control.h"

/* Internal context */
typedef struct {
    const tms320C6452_gpio_pinmux_cfg_t* cfg;
    bool initialized;
} tms320c6452_gpio_pinmux_ctx_t;

static tms320c6452_gpio_pinmux_ctx_t g_pinmux_ctx = { 0 };

/* Helpers */
static inline uint32_t field_mask_u32(uint8_t width, uint8_t lsb)
{
    if (width == 0u) {
        return 0u;
    }
    uint32_t mask;
    if (width >= 32u) {
        mask = 0xFFFFFFFFu;
    } else {
        mask = (1u << width) - 1u;
    }
    mask <<= lsb;
    return mask;
}

static inline uint32_t field_fit_u32(uint32_t value, uint8_t width)
{
    if (width == 0u) {
        return 0u;
    }
    uint32_t mask = (width >= 32u) ? 0xFFFFFFFFu : ((1u << width) - 1u);
    return (value & mask);
}

static inline void reg_write32(volatile uint32_t* addr, uint32_t val)
{
    *addr = val;
    (void)*addr; /* Readback for ordering */
}

static inline uint32_t reg_read32(volatile uint32_t* addr)
{
    return *addr;
}

static inline void reg_update_field32(volatile uint32_t* addr, uint32_t mask, uint32_t shifted_val)
{
    uint32_t cur = reg_read32(addr);
    uint32_t nxt = (cur & ~mask) | (shifted_val & mask);
    reg_write32(addr, nxt);
}

static inline bool validate_field(const tms320c6452_gpio_pinmux_field_t* f)
{
    if (f == NULL) { return false; }
    if (f->width == 0u) { return false; }
    if (f->lsb >= 32u) { return false; }
    if ((uint32_t)f->width + (uint32_t)f->lsb > 32u) { return false; }
    if (f->reg == NULL) { return false; }
    return true;
}

/* API impl */
int tms320c6452_gpio_pinmux_init(const tms320C6452_gpio_pinmux_cfg_t* cfg)
{
    if ((cfg == NULL) || (cfg->map == NULL) || (cfg->count == 0u)) {
        g_pinmux_ctx.initialized = false;
        g_pinmux_ctx.cfg = NULL;
        return TMS320C6452_GPIO_PINMUX_EINVAL;
    }
    /* Basic structural validation only; individual entries validated on use */
    g_pinmux_ctx.cfg = cfg;
    g_pinmux_ctx.initialized = true;
    return TMS320C6452_GPIO_PINMUX_OK;
}

bool tms320c6452_gpio_pinmux_is_initialized(void)
{
    return g_pinmux_ctx.initialized;
}

int tms320c6452_gpio_pinmux_set(uint32_t gpio_index, uint32_t function_code)
{
    if (!g_pinmux_ctx.initialized) {
        return TMS320C6452_GPIO_PINMUX_ESTATE;
    }
    if (g_pinmux_ctx.cfg == NULL || g_pinmux_ctx.cfg->map == NULL) {
        return TMS320C6452_GPIO_PINMUX_ESTATE;
    }
    if (gpio_index >= g_pinmux_ctx.cfg->count) {
        return TMS320C6452_GPIO_PINMUX_EINVAL;
    }

    const tms320c6452_gpio_pinmux_field_t* f = &g_pinmux_ctx.cfg->map[gpio_index];
    if (!validate_field(f)) {
        return TMS320C6452_GPIO_PINMUX_ESTATE;
    }

    uint32_t fit = field_fit_u32(function_code, f->width);
    if (fit != function_code) {
        return TMS320C6452_GPIO_PINMUX_EINVAL; /* function code out of range */
    }

    uint32_t mask = field_mask_u32(f->width, f->lsb);
    uint32_t shifted = (fit << f->lsb);

    reg_update_field32(f->reg, mask, shifted);

    return TMS320C6452_GPIO_PINMUX_OK;
}

int tms320c6452_gpio_pinmux_get(uint32_t gpio_index, uint32_t* out_function_code)
{
    if (!g_pinmux_ctx.initialized) {
        return TMS320C6452_GPIO_PINMUX_ESTATE;
    }
    if (out_function_code == NULL) {
        return TMS320C6452_GPIO_PINMUX_EINVAL;
    }
    if (g_pinmux_ctx.cfg == NULL || g_pinmux_ctx.cfg->map == NULL) {
        return TMS320C6452_GPIO_PINMUX_ESTATE;
    }
    if (gpio_index >= g_pinmux_ctx.cfg->count) {
        return TMS320C6452_GPIO_PINMUX_EINVAL;
    }

    const tms320c6452_gpio_pinmux_field_t* f = &g_pinmux_ctx.cfg->map[gpio_index];
    if (!validate_field(f)) {
        return TMS320C6452_GPIO_PINMUX_ESTATE;
    }

    uint32_t mask = field_mask_u32(f->width, f->lsb);
    uint32_t val = reg_read32(f->reg);
    val = (val & mask) >> f->lsb;
    *out_function_code = field_fit_u32(val, f->width);

    return TMS320C6452_GPIO_PINMUX_OK;
}
