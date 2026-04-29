/*
 * TMS320C6452 DSP GPIO Core Driver - Source
 * Environment: ARM (baremetal), GCC, Big-endian
 * Note: Register offsets are placeholders; verify against device TRM. APIs are stable.
 */
#include "TMS320C6452 DSP_core.h"

/* Volatile MMIO accessors with explicit widths and big-endian ordering */
static inline void mmio_write8(uintptr_t addr, uint8_t v)
{
    volatile uint8_t *p = (volatile uint8_t *)addr;
    *p = v;
}

static inline uint8_t mmio_read8(uintptr_t addr)
{
    volatile const uint8_t *p = (volatile const uint8_t *)addr;
    return *p;
}

static inline void mmio_write16_be(uintptr_t addr, uint16_t v)
{
    uint16_t be = __builtin_bswap16(v);
    volatile uint16_t *p = (volatile uint16_t *)addr;
    *p = be;
}

static inline uint16_t mmio_read16_be(uintptr_t addr)
{
    volatile const uint16_t *p = (volatile const uint16_t *)addr;
    uint16_t be = *p;
    return __builtin_bswap16(be);
}

static inline void mmio_write32_be(uintptr_t addr, uint32_t v)
{
    uint32_t be = __builtin_bswap32(v);
    volatile uint32_t *p = (volatile uint32_t *)addr;
    *p = be;
}

static inline uint32_t mmio_read32_be(uintptr_t addr)
{
    volatile const uint32_t *p = (volatile const uint32_t *)addr;
    uint32_t be = *p;
    return __builtin_bswap32(be);
}

/* Internal helpers */
static inline uint32_t pin_mask(uint32_t pin)
{
    return (uint32_t)1u << (pin & 31u);
}

static inline uintptr_t reg_addr(uintptr_t offset)
{
    return GPIO_REG_ADDR(offset);
}

/* Raw register read/write */
static void core_reg_write32(uintptr_t offset, uint32_t value)
{
    mmio_write32_be(reg_addr(offset), value);
}

static uint32_t core_reg_read32(uintptr_t offset)
{
    return mmio_read32_be(reg_addr(offset));
}

/* Pin operations */
static void core_pin_set(uint32_t pin)
{
    if (pin < GPIO_MAX_PINS)
    {
        core_reg_write32(GPIO_SET_DATA01_OFFSET, pin_mask(pin));
    }
}

static void core_pin_clear(uint32_t pin)
{
    if (pin < GPIO_MAX_PINS)
    {
        core_reg_write32(GPIO_CLR_DATA01_OFFSET, pin_mask(pin));
    }
}

static bool core_pin_read(uint32_t pin)
{
    if (pin < GPIO_MAX_PINS)
    {
        uint32_t v = core_reg_read32(GPIO_IN_DATA01_OFFSET);
        return ( (v & pin_mask(pin)) != 0u );
    }
    return false;
}

static void core_pin_direction_set(uint32_t pin, bool input)
{
    if (pin < GPIO_MAX_PINS)
    {
        uint32_t dir = core_reg_read32(GPIO_DIR01_OFFSET);
        uint32_t m = pin_mask(pin);
        if (input)
        {
            dir |= m;  /* 1 = input */
        }
        else
        {
            dir &= ~m; /* 0 = output */
        }
        core_reg_write32(GPIO_DIR01_OFFSET, dir);
    }
}

static bool core_pin_direction_get(uint32_t pin)
{
    if (pin < GPIO_MAX_PINS)
    {
        uint32_t dir = core_reg_read32(GPIO_DIR01_OFFSET);
        return ( (dir & pin_mask(pin)) != 0u ); /* true=input */
    }
    return true; /* safe default */
}

static uint32_t core_bank_read_inputs(uint32_t bank)
{
    if (bank < GPIO_NUM_BANKS)
    {
        uint32_t v = core_reg_read32(GPIO_IN_DATA01_OFFSET);
        uint32_t shift = (bank * GPIO_PINS_PER_BANK);
        return (v >> shift) & 0xFFFFu;
    }
    return 0u;
}

static uint32_t core_bank_read_outputs(uint32_t bank)
{
    if (bank < GPIO_NUM_BANKS)
    {
        uint32_t v = core_reg_read32(GPIO_OUT_DATA01_OFFSET);
        uint32_t shift = (bank * GPIO_PINS_PER_BANK);
        return (v >> shift) & 0xFFFFu;
    }
    return 0u;
}

/* Public ops instance */
const gpio_core_ops_t gpio_core =
{
    .reg_write32 = core_reg_write32,
    .reg_read32  = core_reg_read32,

    .pin_set = core_pin_set,
    .pin_clear = core_pin_clear,
    .pin_read = core_pin_read,

    .pin_direction_set = core_pin_direction_set,
    .pin_direction_get = core_pin_direction_get,

    .bank_read_inputs = core_bank_read_inputs,
    .bank_read_outputs = core_bank_read_outputs,
};

/* Thin wrapper APIs */
void gpio_reg_write32(uintptr_t offset, uint32_t value)
{
    gpio_core.reg_write32(offset, value);
}

uint32_t gpio_reg_read32(uintptr_t offset)
{
    return gpio_core.reg_read32(offset);
}

void gpio_set_pin(uint32_t pin)
{
    gpio_core.pin_set(pin);
}

void gpio_clear_pin(uint32_t pin)
{
    gpio_core.pin_clear(pin);
}

bool gpio_read_pin(uint32_t pin)
{
    return gpio_core.pin_read(pin);
}

void gpio_set_direction(uint32_t pin, bool input)
{
    gpio_core.pin_direction_set(pin, input);
}

bool gpio_get_direction(uint32_t pin)
{
    return gpio_core.pin_direction_get(pin);
}

uint32_t gpio_read_bank_inputs(uint32_t bank)
{
    return gpio_core.bank_read_inputs(bank);
}

uint32_t gpio_read_bank_outputs(uint32_t bank)
{
    return gpio_core.bank_read_outputs(bank);
}
