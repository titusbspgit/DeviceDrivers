/*
 * Core Driver Header for TMS320C6452 DSP
 * Environment: U-Boot, gcc, big-endian
 * Provides reusable MMIO helpers and function table for feature drivers.
 */
#ifndef TMS320C6452_DSP_CORE_DRV_H
#define TMS320C6452_DSP_CORE_DRV_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* Base address from global configuration */
#define TMS320C6452_DSP_BASE   (0xA1008000u)

/* Bit helpers */
#ifndef BIT
#define BIT(n)                 (1u << (n))
#endif
#ifndef GENMASK
#define GENMASK(h,l)           (((~0u) - (1u << (l)) + 1u) & (~0u >> (31u - (h))))
#endif

/* Raw register accessors */
#define REG8(addr)             (*(volatile uint8_t  *)(uintptr_t)(addr))
#define REG16(addr)            (*(volatile uint16_t *)(uintptr_t)(addr))
#define REG32(addr)            (*(volatile uint32_t *)(uintptr_t)(addr))

/* Address computation from core base + offset */
#define CORE_REG8(off)         REG8 (TMS320C6452_DSP_BASE + (uint32_t)(off))
#define CORE_REG16(off)        REG16(TMS320C6452_DSP_BASE + (uint32_t)(off))
#define CORE_REG32(off)        REG32(TMS320C6452_DSP_BASE + (uint32_t)(off))

/* Endianness conversion helpers (big-endian CPU => no-op) */
#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
#  define BE16_TO_CPU(x) __builtin_bswap16((uint16_t)(x))
#  define BE32_TO_CPU(x) __builtin_bswap32((uint32_t)(x))
#  define CPU_TO_BE16(x) __builtin_bswap16((uint16_t)(x))
#  define CPU_TO_BE32(x) __builtin_bswap32((uint32_t)(x))
#else
#  define BE16_TO_CPU(x) (x)
#  define BE32_TO_CPU(x) (x)
#  define CPU_TO_BE16(x) (x)
#  define CPU_TO_BE32(x) (x)
#endif

/* Core access API */
static inline uint8_t core_read8(uint32_t offset)
{
    return CORE_REG8(offset);
}

static inline void core_write8(uint32_t offset, uint8_t val)
{
    CORE_REG8(offset) = val;
}

static inline uint16_t core_read16(uint32_t offset)
{
    uint16_t v = CORE_REG16(offset);
    return BE16_TO_CPU(v);
}

static inline void core_write16(uint32_t offset, uint16_t val)
{
    CORE_REG16(offset) = CPU_TO_BE16(val);
}

static inline uint32_t core_read32(uint32_t offset)
{
    uint32_t v = CORE_REG32(offset);
    return BE32_TO_CPU(v);
}

static inline void core_write32(uint32_t offset, uint32_t val)
{
    CORE_REG32(offset) = CPU_TO_BE32(val);
}

/* Read-Modify-Write helpers (32-bit) */
static inline void core_set_bits32(uint32_t offset, uint32_t mask)
{
    uint32_t v = core_read32(offset);
    v |= mask;
    core_write32(offset, v);
}

static inline void core_clear_bits32(uint32_t offset, uint32_t mask)
{
    uint32_t v = core_read32(offset);
    v &= ~mask;
    core_write32(offset, v);
}

static inline void core_update_bits32(uint32_t offset, uint32_t mask, uint32_t value)
{
    uint32_t v = core_read32(offset);
    v = (v & ~mask) | (value & mask);
    core_write32(offset, v);
}

/* Function table exposed to feature drivers */
typedef struct {
    uint8_t  (*read8)(uint32_t off);
    void     (*write8)(uint32_t off, uint8_t v);
    uint16_t (*read16)(uint32_t off);
    void     (*write16)(uint32_t off, uint16_t v);
    uint32_t (*read32)(uint32_t off);
    void     (*write32)(uint32_t off, uint32_t v);
    void     (*set_bits32)(uint32_t off, uint32_t mask);
    void     (*clear_bits32)(uint32_t off, uint32_t mask);
    void     (*update_bits32)(uint32_t off, uint32_t mask, uint32_t value);
} tms320c6452_core_ops_t;

/* Singleton accessor (defined in core .c) */
const tms320c6452_core_ops_t *tms320c6452_core_get_ops(void);

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_DSP_CORE_DRV_H */
