/*
 * TMS320C6452 DSP Core Driver Source
 * arch=arm, env=uboot, compiler=gcc, peripheral endianness=bigendian
 * No dynamic memory; C89/C99 portable; MISRA-style discipline.
 */

#include <stdint.h>
#include "TMS320C6452 DSP_core.h"

/* U-Boot microsecond delay */
extern void udelay(unsigned long usec);

/* Compile-time CPU endianness check */
#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
#define RCI_CPU_LITTLE_ENDIAN 1
#else
#define RCI_CPU_LITTLE_ENDIAN 0
#endif

/* 32-bit byte-swap helper */
static uint32_t rci_bswap32(uint32_t v)
{
#if defined(__GNUC__)
    return __builtin_bswap32(v);
#else
    return ((v & 0x000000FFUL) << 24) |
           ((v & 0x0000FF00UL) << 8)  |
           ((v & 0x00FF0000UL) >> 8)  |
           ((v & 0xFF000000UL) >> 24);
#endif
}

/* Core state: mapped base as byte pointer */
static volatile uint8_t *rci_core_base = (volatile uint8_t *)(uintptr_t)0;

/* Internal BE-aware MMIO helpers (32-bit) */
static inline uint32_t rci_read32_be(uint32_t offset)
{
    volatile uint32_t *addr = (volatile uint32_t *)(void *)(rci_core_base + offset);
    uint32_t raw = *addr;
    RCI_MEM_BARRIER();
#if RCI_CPU_LITTLE_ENDIAN
    return rci_bswap32(raw);
#else
    return raw;
#endif
}

static inline void rci_write32_be(uint32_t offset, uint32_t value)
{
    volatile uint32_t *addr = (volatile uint32_t *)(void *)(rci_core_base + offset);
#if RCI_CPU_LITTLE_ENDIAN
    uint32_t v = rci_bswap32(value);
#else
    uint32_t v = value;
#endif
    *addr = v;
    RCI_MEM_BARRIER();
}

/* Public API implementations */
int32_t tms320c6452_dsp_core_init(uint32_t base)
{
    if (base == 0U)
    {
        base = TMS320C6452_DSP_BASE_ADDR;
    }
    rci_core_base = (volatile uint8_t *)(uintptr_t)base;
    return (int32_t)RCI_DRV_OK;
}

void tms320c6452_dsp_core_deinit(void)
{
    rci_core_base = (volatile uint8_t *)(uintptr_t)0;
}

int32_t tms320c6452_dsp_core_reg_read(uint32_t offset, uint32_t *val)
{
    if ((rci_core_base == (volatile uint8_t *)(uintptr_t)0) || (val == (uint32_t *)0))
    {
        return (int32_t)RCI_DRV_EINVAL;
    }
    *val = rci_read32_be(offset);
    return (int32_t)RCI_DRV_OK;
}

int32_t tms320c6452_dsp_core_reg_write(uint32_t offset, uint32_t v)
{
    if (rci_core_base == (volatile uint8_t *)(uintptr_t)0)
    {
        return (int32_t)RCI_DRV_EINVAL;
    }
    rci_write32_be(offset, v);
    return (int32_t)RCI_DRV_OK;
}

int32_t tms320c6452_dsp_core_set_bits(uint32_t offset, uint32_t mask)
{
    if (rci_core_base == (volatile uint8_t *)(uintptr_t)0)
    {
        return (int32_t)RCI_DRV_EINVAL;
    }
    uint32_t val = rci_read32_be(offset);
    val |= mask;
    rci_write32_be(offset, val);
    return (int32_t)RCI_DRV_OK;
}

int32_t tms320c6452_dsp_core_clear_bits(uint32_t offset, uint32_t mask)
{
    if (rci_core_base == (volatile uint8_t *)(uintptr_t)0)
    {
        return (int32_t)RCI_DRV_EINVAL;
    }
    uint32_t val = rci_read32_be(offset);
    val &= ~mask;
    rci_write32_be(offset, val);
    return (int32_t)RCI_DRV_OK;
}

int32_t tms320c6452_dsp_core_update_bits(uint32_t offset, uint32_t mask, uint32_t value)
{
    if (rci_core_base == (volatile uint8_t *)(uintptr_t)0)
    {
        return (int32_t)RCI_DRV_EINVAL;
    }
    uint32_t val = rci_read32_be(offset);
    val = (val & ~mask) | (value & mask);
    rci_write32_be(offset, val);
    return (int32_t)RCI_DRV_OK;
}

void tms320c6452_dsp_core_delay_us(uint32_t usec)
{
    udelay((unsigned long)usec);
}

const tms320c6452_dsp_core_ops_t tms320c6452_dsp_core_ops =
{
    tms320c6452_dsp_core_init,
    tms320c6452_dsp_core_deinit,
    tms320c6452_dsp_core_reg_read,
    tms320c6452_dsp_core_reg_write,
    tms320c6452_dsp_core_set_bits,
    tms320c6452_dsp_core_clear_bits,
    tms320c6452_dsp_core_update_bits,
    tms320c6452_dsp_core_delay_us
};
