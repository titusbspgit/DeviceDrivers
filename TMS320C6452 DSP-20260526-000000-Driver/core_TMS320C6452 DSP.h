/*
 * core_TMS320C6452 DSP.h
 * Reusable core access layer for TMS320C6452 DSP peripherals
 * Big-endian, bare-metal, GCC
 */
#ifndef CORE_TMS320C6452_DSP_H
#define CORE_TMS320C6452_DSP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

/* Endianness configuration (fixed by environment) */
#define CORE_TMS320C6452_DSP_BIG_ENDIAN 1u

/* Base address (from configuration) */
#define CORE_TMS320C6452_DSP_BASE (0xA1008000u)

/* Example register offsets (placeholders for feature drivers) */
#define CORE_TMS320C6452_DSP_EXAMPLE_REG0_OFFSET  (0x0000u)
#define CORE_TMS320C6452_DSP_EXAMPLE_REG1_OFFSET  (0x0004u)
#define CORE_TMS320C6452_DSP_EXAMPLE_REG2_OFFSET  (0x0008u)
#define CORE_TMS320C6452_DSP_EXAMPLE_REG3_OFFSET  (0x000Cu)

/* Address helper */
#define CORE_TMS320C6452_DSP_REG_ADDR(off) ((uintptr_t)(CORE_TMS320C6452_DSP_BASE + (uintptr_t)(off)))

/* Byte-swap helpers (portable) */
static inline uint16_t core_tms320c6452_dsp_bswap16(uint16_t v)
{
    return (uint16_t)((v >> 8) | (v << 8));
}

static inline uint32_t core_tms320c6452_dsp_bswap32(uint32_t v)
{
    return ((v & 0x000000FFu) << 24) |
           ((v & 0x0000FF00u) << 8)  |
           ((v & 0x00FF0000u) >> 8)  |
           ((v & 0xFF000000u) >> 24);
}

/* Memory barrier (compiler) */
static inline void core_tms320c6452_dsp_barrier(void)
{
#if defined(__GNUC__)
    __asm__ __volatile__("" ::: "memory");
#else
    /* Fallback: volatile barrier via dummy */
    volatile uint32_t __barrier = 0u; (void)__barrier;
#endif
}

/* Volatile MMIO accessors (8/16/32-bit) with big-endian handling */
static inline void core_tms320c6452_dsp_write8(uintptr_t addr, uint8_t value)
{
    *(volatile uint8_t *)addr = value;
    core_tms320c6452_dsp_barrier();
}

static inline uint8_t core_tms320c6452_dsp_read8(uintptr_t addr)
{
    uint8_t v = *(volatile const uint8_t *)addr;
    core_tms320c6452_dsp_barrier();
    return v;
}

static inline void core_tms320c6452_dsp_write16(uintptr_t addr, uint16_t value)
{
#if CORE_TMS320C6452_DSP_BIG_ENDIAN
    value = core_tms320c6452_dsp_bswap16(value);
#endif
    *(volatile uint16_t *)addr = value;
    core_tms320c6452_dsp_barrier();
}

static inline uint16_t core_tms320c6452_dsp_read16(uintptr_t addr)
{
    uint16_t v = *(volatile const uint16_t *)addr;
#if CORE_TMS320C6452_DSP_BIG_ENDIAN
    v = core_tms320c6452_dsp_bswap16(v);
#endif
    core_tms320c6452_dsp_barrier();
    return v;
}

static inline void core_tms320c6452_dsp_write32(uintptr_t addr, uint32_t value)
{
#if CORE_TMS320C6452_DSP_BIG_ENDIAN
    value = core_tms320c6452_dsp_bswap32(value);
#endif
    *(volatile uint32_t *)addr = value;
    core_tms320c6452_dsp_barrier();
}

static inline uint32_t core_tms320c6452_dsp_read32(uintptr_t addr)
{
    uint32_t v = *(volatile const uint32_t *)addr;
#if CORE_TMS320C6452_DSP_BIG_ENDIAN
    v = core_tms320c6452_dsp_bswap32(v);
#endif
    core_tms320c6452_dsp_barrier();
    return v;
}

/* Core API structure for feature drivers */
typedef struct core_tms320c6452_dsp_api {
    uintptr_t base;
    void     (*write8)(uintptr_t addr, uint8_t value);
    uint8_t  (*read8)(uintptr_t addr);
    void     (*write16)(uintptr_t addr, uint16_t value);
    uint16_t (*read16)(uintptr_t addr);
    void     (*write32)(uintptr_t addr, uint32_t value);
    uint32_t (*read32)(uintptr_t addr);
    int      (*init)(void);
    void     (*deinit)(void);
} core_tms320c6452_dsp_api_t;

/* Accessor for the singleton API instance */
const core_tms320c6452_dsp_api_t* core_tms320c6452_dsp_get_api(void);

#ifdef __cplusplus
}
#endif

#endif /* CORE_TMS320C6452_DSP_H */
