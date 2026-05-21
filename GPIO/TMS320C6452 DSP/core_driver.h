#ifndef CORE_DRIVER_H
#define CORE_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Core register I/O helpers for memory-mapped peripherals
 * Target: Baremetal, GCC, ARM, Big-Endian
 * Base address is provided by integration; defaulted here per global config.
 */
#include <stdint.h>

#ifndef CORE_BASE_ADDR
#define CORE_BASE_ADDR (0xA1008000u)
#endif

/* Addressing helpers */
#define CORE_REG32(off)      (*(volatile uint32_t *)((uintptr_t)(CORE_BASE_ADDR) + (uint32_t)(off)))

/* Compiler/CPU barriers */
#if defined(__GNUC__)
  #define CORE_COMPILER_BARRIER() __asm__ __volatile__("" ::: "memory")
  /* Best-effort DMB; if unsupported, at least enforce compiler ordering */
  #if defined(__arm__) || defined(__aarch64__)
    #define CORE_DMB() __asm__ __volatile__("dmb sy" ::: "memory")
  #else
    #define CORE_DMB() CORE_COMPILER_BARRIER()
  #endif
#else
  #define CORE_COMPILER_BARRIER() do { } while (0)
  #define CORE_DMB()              do { } while (0)
#endif

static inline uint32_t core_read32(uint32_t off)
{
    CORE_COMPILER_BARRIER();
    uint32_t v = CORE_REG32(off);
    CORE_DMB();
    return v;
}

static inline void core_write32(uint32_t off, uint32_t val)
{
    CORE_COMPILER_BARRIER();
    CORE_REG32(off) = val;
    CORE_DMB();
}

static inline void core_set_bits(uint32_t off, uint32_t mask)
{
    uint32_t v = core_read32(off);
    core_write32(off, v | mask);
}

static inline void core_clr_bits(uint32_t off, uint32_t mask)
{
    uint32_t v = core_read32(off);
    core_write32(off, v & ~mask);
}

/* Poll until (reg & mask) == expected, or timeout (in cycles/iterations) elapses. Returns 0 on success, -1 on timeout. */
static inline int core_poll_bits(uint32_t off, uint32_t mask, uint32_t expected, uint32_t timeout_cycles)
{
    while (timeout_cycles--) {
        uint32_t v = core_read32(off) & mask;
        if (v == (expected & mask)) {
            return 0;
        }
    }
    return -1;
}

/* Optional API object for consumers preferring indirect calls */
typedef struct drv_core_api {
    uint32_t (*read32)(uint32_t off);
    void     (*write32)(uint32_t off, uint32_t val);
    void     (*set_bits)(uint32_t off, uint32_t mask);
    void     (*clr_bits)(uint32_t off, uint32_t mask);
    int      (*poll_bits)(uint32_t off, uint32_t mask, uint32_t expected, uint32_t timeout_cycles);
} drv_core_api_t;

const drv_core_api_t* drv_core(void);

#ifdef __cplusplus
}
#endif

#endif /* CORE_DRIVER_H */
