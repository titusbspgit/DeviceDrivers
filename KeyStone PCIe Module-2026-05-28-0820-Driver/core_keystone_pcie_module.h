#ifndef CORE_KEYSTONE_PCIE_MODULE_H
#define CORE_KEYSTONE_PCIE_MODULE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <inttypes.h>

#ifndef KPM_ARCH_ARM
#define KPM_ARCH_ARM 1
#endif

#ifndef KPM_BIG_ENDIAN
#define KPM_BIG_ENDIAN 1
#endif

#ifndef KPM_PCIE_BASE
#define KPM_PCIE_BASE ((uintptr_t)0xA1008000u)
#endif

#if KPM_ARCH_ARM
#define KPM_DMB() __asm__ __volatile__("dmb sy" ::: "memory")
#define KPM_DSB() __asm__ __volatile__("dsb sy" ::: "memory")
#define KPM_ISB() __asm__ __volatile__("isb" ::: "memory")
#else
#define KPM_DMB() __sync_synchronize()
#define KPM_DSB() __sync_synchronize()
#define KPM_ISB() do { } while (0)
#endif

static inline uint32_t kpm_read32(uintptr_t offset)
{
    volatile uint32_t *addr = (volatile uint32_t *)(KPM_PCIE_BASE + offset);
    uint32_t v = *addr;
    return v;
}

static inline void kpm_write32(uintptr_t offset, uint32_t value)
{
    volatile uint32_t *addr = (volatile uint32_t *)(KPM_PCIE_BASE + offset);
    *addr = value;
}

static inline void kpm_mb(void)
{
    KPM_DMB();
}

__attribute__((weak)) void kpm_udelay(uint32_t usec)
{
    volatile uint32_t i;
    for (i = 0; i < (usec * 100u); ++i) {
        __asm__ __volatile__("nop");
    }
}

typedef enum {
    KPM_STATUS_OK = 0,
    KPM_STATUS_ERROR = -1,
    KPM_STATUS_INVALID_ARG = -2,
    KPM_STATUS_TIMEOUT = -3,
    KPM_STATUS_UNSUPPORTED = -4,
    KPM_STATUS_NOT_IMPLEMENTED = -5
} kpm_status_t;

typedef struct kpm_core_api_s {
    uintptr_t base;
    uint32_t (*read32)(uintptr_t offset);
    void     (*write32)(uintptr_t offset, uint32_t value);
    void     (*mb)(void);
    void     (*udelay)(uint32_t usec);
} kpm_core_api_t;

const kpm_core_api_t *kpm_core_get_default(void);

#ifdef __cplusplus
}
#endif

#endif /* CORE_KEYSTONE_PCIE_MODULE_H */
