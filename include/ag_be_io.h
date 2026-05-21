#ifndef AG_BE_IO_H
#define AG_BE_IO_H

#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

static inline void writel_be(volatile void *addr, uint32_t val)
{
#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
    // Swap to big-endian before store on little-endian host
    uint32_t be = __builtin_bswap32(val);
    *(volatile uint32_t *)addr = be;
#else
    *(volatile uint32_t *)addr = val;
#endif
}

static inline uint32_t readl_be(volatile void *addr)
{
#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
    uint32_t be = *(volatile uint32_t *)addr;
    return __builtin_bswap32(be);
#else
    return *(volatile uint32_t *)addr;
#endif
}

#if defined(__cplusplus)
}
#endif

#endif // AG_BE_IO_H
