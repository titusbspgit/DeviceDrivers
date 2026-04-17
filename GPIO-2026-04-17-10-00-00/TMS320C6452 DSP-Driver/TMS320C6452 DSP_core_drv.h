/*
 * Core Driver Header for TMS320C6452 DSP
 * Environment: Baremetal, gcc, big-endian
 * Provides reusable MMIO helpers and function table for feature drivers.
 */
#ifndef TMS320C6452_DSP_CORE_DRV_H
#define TMS320C6452_DSP_CORE_DRV_H

#ifdef __cplusplus
extern "C" {
#endif

/* Configured base address */
#define TMS320C6452_DSP_BASE   (0xA1008000u)

/* Generic register accessors */
#define REG32(addr)            (*(volatile unsigned int *)(addr))
#define REG16(addr)            (*(volatile unsigned short *)(addr))
#define REG8(addr)             (*(volatile unsigned char *)(addr))

/* Address computation from configured base */
#define TMS320C6452_DSP_REG32(off)   REG32(TMS320C6452_DSP_BASE + (unsigned int)(off))
#define TMS320C6452_DSP_REG16(off)   REG16(TMS320C6452_DSP_BASE + (unsigned int)(off))
#define TMS320C6452_DSP_REG8(off)    REG8(TMS320C6452_DSP_BASE + (unsigned int)(off))

/* Bit helpers */
#define BIT(n)                 (1u << (n))
#define GENMASK(h,l)           (((~0u) - (1u << (l)) + 1u) & (~0u >> (31u - (h))))

/* Big-endian aware byte swap helpers (no-ops on big-endian) */
#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
#  define BE16_TO_CPU(x) __builtin_bswap16((unsigned short)(x))
#  define BE32_TO_CPU(x) __builtin_bswap32((unsigned int)(x))
#  define CPU_TO_BE16(x) __builtin_bswap16((unsigned short)(x))
#  define CPU_TO_BE32(x) __builtin_bswap32((unsigned int)(x))
#else
#  define BE16_TO_CPU(x) (x)
#  define BE32_TO_CPU(x) (x)
#  define CPU_TO_BE16(x) (x)
#  define CPU_TO_BE32(x) (x)
#endif

/* Core access API typedefs */
typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;

static inline u8 core_read8(u32 offset)
{
    return TMS320C6452_DSP_REG8(offset);
}

static inline void core_write8(u32 offset, u8 val)
{
    TMS320C6452_DSP_REG8(offset) = val;
}

static inline u16 core_read16(u32 offset)
{
    u16 v = TMS320C6452_DSP_REG16(offset);
    return BE16_TO_CPU(v);
}

static inline void core_write16(u32 offset, u16 val)
{
    TMS320C6452_DSP_REG16(offset) = CPU_TO_BE16(val);
}

static inline u32 core_read32(u32 offset)
{
    u32 v = TMS320C6452_DSP_REG32(offset);
    return BE32_TO_CPU(v);
}

static inline void core_write32(u32 offset, u32 val)
{
    TMS320C6452_DSP_REG32(offset) = CPU_TO_BE32(val);
}

/* Read-modify-write helpers */
static inline void core_set_bits32(u32 offset, u32 mask)
{
    u32 v = core_read32(offset);
    v |= mask;
    core_write32(offset, v);
}

static inline void core_clear_bits32(u32 offset, u32 mask)
{
    u32 v = core_read32(offset);
    v &= ~mask;
    core_write32(offset, v);
}

/* Function table exposed to feature drivers */
typedef struct {
    u8  (*read8)(u32 off);
    void (*write8)(u32 off, u8 v);
    u16 (*read16)(u32 off);
    void (*write16)(u32 off, u16 v);
    u32 (*read32)(u32 off);
    void (*write32)(u32 off, u32 v);
    void (*set_bits32)(u32 off, u32 mask);
    void (*clear_bits32)(u32 off, u32 mask);
} tms320c6452_core_ops_t;

/* Singleton accessor for ops (defined in core .c) */
const tms320c6452_core_ops_t *tms320c6452_core_get_ops(void);

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_DSP_CORE_DRV_H */
