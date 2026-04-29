/*
 * Core MMIO support for TMS320C6452 DSP (big-endian aware)
 * Environment: Baremetal, gcc, big-endian
 * NOTE: Unknown register maps should be filled later. Placeholders marked as UNKNOWN.
 */
#ifndef TMS320C6452_DSP_CORE_H
#define TMS320C6452_DSP_CORE_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TMS320C6452_DSP_BASE   (0xA1008000u)

/* Endian conversion helpers (deterministic, no dynamic allocation) */
static inline uint16_t core_be16_to_cpu(uint16_t v)
{
#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
    return (uint16_t)((v >> 8) | (v << 8));
#else
    return v;
#endif
}

static inline uint32_t core_be32_to_cpu(uint32_t v)
{
#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
    return __builtin_bswap32(v);
#else
    return v;
#endif
}

static inline uint16_t core_cpu_to_be16(uint16_t v)
{
#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
    return (uint16_t)((v >> 8) | (v << 8));
#else
    return v;
#endif
}

static inline uint32_t core_cpu_to_be32(uint32_t v)
{
#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
    return __builtin_bswap32(v);
#else
    return v;
#endif
}

/* Memory barriers (placeholders for baremetal) */
static inline void core_dmb(void) { __asm__ __volatile__("" : : : "memory"); }
static inline void core_dsb(void) { __asm__ __volatile__("" : : : "memory"); }
static inline void core_isb(void) { __asm__ __volatile__("" : : : "memory"); }

/* Big-endian MMIO accessors */
static inline uint32_t core_mmio_read32_be(volatile void const *addr)
{
    uint32_t v = *(volatile uint32_t const *)addr; /* deterministic */
    return core_be32_to_cpu(v);
}

static inline void core_mmio_write32_be(volatile void *addr, uint32_t v)
{
    *(volatile uint32_t *)addr = core_cpu_to_be32(v);
}

static inline void core_mmio_set_bits32_be(volatile void *addr, uint32_t set_mask)
{
    uint32_t v = core_mmio_read32_be(addr);
    v |= set_mask;
    core_mmio_write32_be(addr, v);
}

static inline void core_mmio_clear_bits32_be(volatile void *addr, uint32_t clr_mask)
{
    uint32_t v = core_mmio_read32_be(addr);
    v &= (uint32_t)(~clr_mask);
    core_mmio_write32_be(addr, v);
}

/* Minimal delay stub (UNKNOWN timing). Deterministic busy-wait. */
static inline void core_delay_cycles(volatile uint32_t cycles)
{
    while (cycles-- != 0u) {
        __asm__ __volatile__("" : : : "memory");
    }
}

/* Core ops to be used by feature drivers */
typedef struct
{
    uint32_t (*read32)(volatile void const *addr);
    void     (*write32)(volatile void *addr, uint32_t v);
    void     (*set_bits32)(volatile void *addr, uint32_t m);
    void     (*clear_bits32)(volatile void *addr, uint32_t m);
    void     (*delay_cycles)(volatile uint32_t cycles);
    void     (*dmb)(void);
    void     (*dsb)(void);
    void     (*isb)(void);
} tms320c6452_dsp_core_ops_t;

extern const tms320c6452_dsp_core_ops_t tms320c6452_dsp_core_ops;

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_DSP_CORE_H */
