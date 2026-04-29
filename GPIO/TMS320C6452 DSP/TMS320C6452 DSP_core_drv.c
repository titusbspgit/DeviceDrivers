/*
 * TMS320C6452 DSP GPIO Core Driver - Source
 * Environment: ARM (baremetal), GCC, Big-endian
 * Note: Register offsets are placeholders; verify against device TRM. APIs are stable.
 */
#include "TMS320C6452 DSP_core.h"

/* Endianness handling: perform bswap only when compiling for little-endian */
#ifndef __BYTE_ORDER__
  #error "Compiler byte order macro __BYTE_ORDER__ not defined"
#endif

/* Volatile MMIO accessors with explicit widths */
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

static inline void mmio_write16(uintptr_t addr, uint16_t v)
{
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    v = __builtin_bswap16(v);
#endif
    volatile uint16_t *p = (volatile uint16_t *)addr;
    *p = v;
}

static inline uint16_t mmio_read16(uintptr_t addr)
{
    volatile const uint16_t *p = (volatile const uint16_t *)addr;
    uint16_t v = *p;
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    v = __builtin_bswap16(v);
#endif
    return v;
}

static inline void mmio_write32(uintptr_t addr, uint32_t v)
{
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    v = __builtin_bswap32(v);
#endif
    volatile uint32_t *p = (volatile uint32_t *)addr;
    *p = v;
}

static inline uint32_t mmio_read32(uintptr_t addr)
{
    volatile const uint32_t *p = (volatile const uint32_t *)addr;
    uint32_t v = *p;
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    v = __builtin_bswap32(v);
#endif
    return v;
}

/* Optional compiler I/O barrier for ordering */
void gpio_io_barrier(void)
{
    __asm__ __volatile__("" ::: "memory");
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

/* Raw register read/write wrappers exposed via ops */
static void core_reg_write8(uintptr_t offset, uint8_t value)
{
    mmio_write8(reg_addr(offset), value);
}

static uint8_t core_reg_read8(uintptr_t offset)
{
    return mmio_read8(reg_addr(offset));
}

static void core_reg_write16(uintptr_t offset, uint16_t value)
{
    mmio_write16(reg_addr(offset), value);
}

static uint16_t core_reg_read16(uintptr_t offset)
{
    return mmio_read16(reg_addr(offset));
}

static void core_reg_write32(uintptr_t offset, uint32_t value)
{
    mmio_write32(reg_addr(offset), value);
}

static uint32_t core_reg_read32(uintptr_t offset)
{
    return mmio_read32(reg_addr(offset));
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

static void core_bank_set_mask(uint32_t bank, uint16_t mask)
{
    if (bank < GPIO_NUM_BANKS)
    {
        uint32_t shift = (bank * GPIO_PINS_PER_BANK);
        core_reg_write32(GPIO_SET_DATA01_OFFSET, ((uint32_t)mask) << shift);
    }
}

static void core_bank_clear_mask(uint32_t bank, uint16_t mask)
{
    if (bank < GPIO_NUM_BANKS)
    {
        uint32_t shift = (bank * GPIO_PINS_PER_BANK);
        core_reg_write32(GPIO_CLR_DATA01_OFFSET, ((uint32_t)mask) << shift);
    }
}

/* Public ops instance */
const gpio_core_ops_t gpio_core =
{
    .reg_write8  = core_reg_write8,
    .reg_read8   = core_reg_read8,
    .reg_write16 = core_reg_write16,
    .reg_read16  = core_reg_read16,
    .reg_write32 = core_reg_write32,
    .reg_read32  = core_reg_read32,

    .pin_set = core_pin_set,
    .pin_clear = core_pin_clear,
    .pin_read = core_pin_read,

    .pin_direction_set = core_pin_direction_set,
    .pin_direction_get = core_pin_direction_get,

    .bank_read_inputs = core_bank_read_inputs,
    .bank_read_outputs = core_bank_read_outputs,
    .bank_set_mask = core_bank_set_mask,
    .bank_clear_mask = core_bank_clear_mask,
};

/* Thin wrapper APIs */
void gpio_io_barrier(void)
{
    __asm__ __volatile__("" ::: "memory");
}

void gpio_reg_write8(uintptr_t offset, uint8_t value)
{
    gpio_core.reg_write8(offset, value);
}

uint8_t gpio_reg_read8(uintptr_t offset)
{
    return gpio_core.reg_read8(offset);
}

void gpio_reg_write16(uintptr_t offset, uint16_t value)
{
    gpio_core.reg_write16(offset, value);
}

uint16_t gpio_reg_read16(uintptr_t offset)
{
    return gpio_core.reg_read16(offset);
}

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

void gpio_bank_set_mask(uint32_t bank, uint16_t mask)
{
    gpio_core.bank_set_mask(bank, mask);
}

void gpio_bank_clear_mask(uint32_t bank, uint16_t mask)
{
    gpio_core.bank_clear_mask(bank, mask);
}
