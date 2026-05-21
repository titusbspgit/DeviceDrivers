/* core_TMS320C6452_DSP.h - Core MMIO helpers for TMS320C6452 DSP
 * Env: Baremetal, gcc (C11), Arch: arm, Endianness: big-endian target
 */
#ifndef CORE_TMS320C6452_DSP_H
#define CORE_TMS320C6452_DSP_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Controller base address (from global configuration) */
#define TMS320C6452_DSP_BASE_ADDR ((uintptr_t)0xA1008000u)

/* Endianness model: target registers are big-endian */
#define CORE_MMIO_TARGET_BIG_ENDIAN 1

#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#define CORE_MMIO_HOST_BIG_ENDIAN 1
#else
#define CORE_MMIO_HOST_BIG_ENDIAN 0
#endif

/* Byte-swap helpers for portability */
static inline uint16_t core_bswap16(uint16_t v) {
#if defined(__has_builtin)
#  if __has_builtin(__builtin_bswap16)
    return __builtin_bswap16(v);
#  else
    return (uint16_t)((v << 8) | (v >> 8));
#  endif
#else
    return (uint16_t)((v << 8) | (v >> 8));
#endif
}

static inline uint32_t core_bswap32(uint32_t v) {
#if defined(__has_builtin)
#  if __has_builtin(__builtin_bswap32)
    return __builtin_bswap32(v);
#  else
    return ((v & 0x000000FFu) << 24) |
           ((v & 0x0000FF00u) << 8)  |
           ((v & 0x00FF0000u) >> 8)  |
           ((v & 0xFF000000u) >> 24);
#  endif
#else
    return ((v & 0x000000FFu) << 24) |
           ((v & 0x0000FF00u) << 8)  |
           ((v & 0x00FF0000u) >> 8)  |
           ((v & 0xFF000000u) >> 24);
#endif
}

/* MMIO accessors (volatile, ordered). If host and target endianness differ, swap. */
static inline void core_write8(uintptr_t addr, uint8_t v) {
    *(volatile uint8_t *)addr = v;
}

static inline void core_write16(uintptr_t addr, uint16_t v) {
#if CORE_MMIO_TARGET_BIG_ENDIAN == CORE_MMIO_HOST_BIG_ENDIAN
    *(volatile uint16_t *)addr = v;
#else
    *(volatile uint16_t *)addr = core_bswap16(v);
#endif
}

static inline void core_write32(uintptr_t addr, uint32_t v) {
#if CORE_MMIO_TARGET_BIG_ENDIAN == CORE_MMIO_HOST_BIG_ENDIAN
    *(volatile uint32_t *)addr = v;
#else
    *(volatile uint32_t *)addr = core_bswap32(v);
#endif
}

static inline uint8_t core_read8(uintptr_t addr) {
    return *(volatile const uint8_t *)addr;
}

static inline uint16_t core_read16(uintptr_t addr) {
#if CORE_MMIO_TARGET_BIG_ENDIAN == CORE_MMIO_HOST_BIG_ENDIAN
    return *(volatile const uint16_t *)addr;
#else
    return core_bswap16(*(volatile const uint16_t *)addr);
#endif
}

static inline uint32_t core_read32(uintptr_t addr) {
#if CORE_MMIO_TARGET_BIG_ENDIAN == CORE_MMIO_HOST_BIG_ENDIAN
    return *(volatile const uint32_t *)addr;
#else
    return core_bswap32(*(volatile const uint32_t *)addr);
#endif
}

/* Optional barriers for strict ordering on some ARM cores */
static inline void core_dmb(void) { __asm__ __volatile__("dmb ish" ::: "memory"); }
static inline void core_dsb(void) { __asm__ __volatile__("dsb ish" ::: "memory"); }
static inline void core_isb(void) { __asm__ __volatile__("isb" ::: "memory"); }

/* Core ops interface that feature drivers can use */
typedef struct core_tms320c6452_ops {
    void     (*write8)(uintptr_t, uint8_t);
    void     (*write16)(uintptr_t, uint16_t);
    void     (*write32)(uintptr_t, uint32_t);
    uint8_t  (*read8)(uintptr_t);
    uint16_t (*read16)(uintptr_t);
    uint32_t (*read32)(uintptr_t);
    void     (*dmb)(void);
    void     (*dsb)(void);
    void     (*isb)(void);
    uintptr_t base;
} core_tms320c6452_ops_t;

const core_tms320c6452_ops_t *core_tms320c6452_get_ops(void);

#ifdef __cplusplus
}
#endif

#endif /* CORE_TMS320C6452_DSP_H */
