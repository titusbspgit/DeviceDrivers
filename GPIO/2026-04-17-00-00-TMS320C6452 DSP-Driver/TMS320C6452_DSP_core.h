#ifndef TMS320C6452_DSP_CORE_H
#define TMS320C6452_DSP_CORE_H

// Baremetal core access header for TMS320C6452 DSP
// Environment: arch=arm, compiler=gcc, endianness=bigendian

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifndef __has_builtin
  #define __has_builtin(x) 0
#endif

// Base address configured
#define TMS320C6452_DSP_BASE_ADDR ((uintptr_t)0xA1008000u)

// Unknown register placeholder offset when details are unavailable
#define TMS320C6452_DSP_REG_UNKNOWN (0x0000u) /* UNKNOWN */

// Compute absolute register address from offset
static inline uintptr_t TMS320C6452_DSP_REG(uintptr_t offset)
{
    return (TMS320C6452_DSP_BASE_ADDR + offset);
}

// Big-endian aware 8/16/32-bit MMIO accessors
static inline uint8_t tms320c6452_dsp_read8(uintptr_t addr)
{
    return *(volatile uint8_t *)(addr);
}

static inline void tms320c6452_dsp_write8(uintptr_t addr, uint8_t val)
{
    *(volatile uint8_t *)(addr) = val;
}

static inline uint16_t tms320c6452_dsp_read16_be(uintptr_t addr)
{
    uint16_t v = *(volatile uint16_t *)(addr);
#if defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
  #if __has_builtin(__builtin_bswap16)
    v = __builtin_bswap16(v);
  #else
    v = (uint16_t)((v >> 8) | (v << 8));
  #endif
#endif
    return v;
}

static inline void tms320c6452_dsp_write16_be(uintptr_t addr, uint16_t val)
{
#if defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
  #if __has_builtin(__builtin_bswap16)
    val = __builtin_bswap16(val);
  #else
    val = (uint16_t)((val >> 8) | (val << 8));
  #endif
#endif
    *(volatile uint16_t *)(addr) = val;
}

static inline uint32_t tms320c6452_dsp_read32_be(uintptr_t addr)
{
    uint32_t v = *(volatile uint32_t *)(addr);
#if defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
  #if __has_builtin(__builtin_bswap32)
    v = __builtin_bswap32(v);
  #else
    v = ((v & 0x000000FFu) << 24) | ((v & 0x0000FF00u) << 8) |
        ((v & 0x00FF0000u) >> 8)  | ((v & 0xFF000000u) >> 24);
  #endif
#endif
    return v;
}

static inline void tms320c6452_dsp_write32_be(uintptr_t addr, uint32_t val)
{
#if defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
  #if __has_builtin(__builtin_bswap32)
    val = __builtin_bswap32(val);
  #else
    val = ((val & 0x000000FFu) << 24) | ((val & 0x0000FF00u) << 8) |
          ((val & 0x00FF0000u) >> 8)  | ((val & 0xFF000000u) >> 24);
  #endif
#endif
    *(volatile uint32_t *)(addr) = val;
}

// Core API function table for feature drivers
typedef struct {
    uintptr_t base;
    uint8_t  (*rd8)(uintptr_t addr);
    uint16_t (*rd16)(uintptr_t addr);
    uint32_t (*rd32)(uintptr_t addr);
    void     (*wr8)(uintptr_t addr, uint8_t v);
    void     (*wr16)(uintptr_t addr, uint16_t v);
    void     (*wr32)(uintptr_t addr, uint32_t v);
} TMS320C6452_DSP_CoreAPI;

// Retrieve pointer to core API (singleton)
const TMS320C6452_DSP_CoreAPI *TMS320C6452_DSP_core_get(void);

#endif /* TMS320C6452_DSP_CORE_H */
