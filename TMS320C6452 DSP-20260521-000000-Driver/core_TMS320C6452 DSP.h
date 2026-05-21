/* Core driver for TMS320C6452 DSP (Baremetal, gcc, big-endian) */
#ifndef CORE_TMS320C6452_DSP_H
#define CORE_TMS320C6452_DSP_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <inttypes.h>

/* Peripheral base address supplied via configuration */
#ifndef PERIPH_BASE
#define PERIPH_BASE ((uintptr_t)0xA1008000u)
#endif

/* Endianness helpers: handle big-endian register space */
#ifndef CORE_ENDIAN_HELPERS
#define CORE_ENDIAN_HELPERS
static inline uint16_t core_be16_to_cpu(uint16_t v){
#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
    return (uint16_t)__builtin_bswap16(v);
#else
    return v;
#endif
}
static inline uint32_t core_be32_to_cpu(uint32_t v){
#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
    return __builtin_bswap32(v);
#else
    return v;
#endif
}
static inline uint16_t core_cpu_to_be16(uint16_t v){
#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
    return (uint16_t)__builtin_bswap16(v);
#else
    return v;
#endif
}
static inline uint32_t core_cpu_to_be32(uint32_t v){
#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
    return __builtin_bswap32(v);
#else
    return v;
#endif
}
#endif /* CORE_ENDIAN_HELPERS */

/* Register address helpers */
#define REG_ADDR(off)  ((uintptr_t)(PERIPH_BASE + (uintptr_t)(off)))
#define REG8(off)      (*(volatile uint8_t*) REG_ADDR(off))
#define REG16(off)     (*(volatile uint16_t*)REG_ADDR(off))
#define REG32(off)     (*(volatile uint32_t*)REG_ADDR(off))

/* Minimal barrier for MMIO ordering on baremetal gcc */
static inline void core_mb(void){ __asm__ __volatile__("" ::: "memory"); }

/*
 * Known GPIO register offsets from provided spec snippets
 * NOTE: Only offsets explicitly provided are defined here to avoid hallucination.
 */
#define GPIO_DIR01_OFFSET   (0x10u) /* SPRUF95 Table 2: DIR01 */

/* Convenience absolute addresses */
#define GPIO_DIR01_ADDR     REG_ADDR(GPIO_DIR01_OFFSET)

/* Core API exposed to feature drivers */
typedef struct core_api_s {
    uint32_t (*read32)(uintptr_t addr);
    void     (*write32)(uintptr_t addr, uint32_t value);
    void     (*rmw32)(uintptr_t addr, uint32_t set_mask, uint32_t clear_mask);
    void     (*delay_cycles)(uint32_t cycles);
} core_api_t;

/* Populate a core_api_t with function pointers */
void core_get_api(core_api_t* api);

#endif /* CORE_TMS320C6452_DSP_H */
