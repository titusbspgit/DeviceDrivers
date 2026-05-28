#ifndef KEYSTONE_PCIE_MODULE_DUAL_MODE_OPERATION_ROOT_COMPLEX_AND_END_POINT_H
#define KEYSTONE_PCIE_MODULE_DUAL_MODE_OPERATION_ROOT_COMPLEX_AND_END_POINT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Base address */
#define KPM_BASE_ADDR              (0xA1008000u)

/* Unknown placeholders per specification constraints */
#define UNKNOWN                    (0xFFFFFFFFu)

/* Register offsets (UNKNOWN when not available) */
#define KPM_MODE_REG_OFFSET        (UNKNOWN)
#define KPM_LTSSM_CTRL_OFFSET      (UNKNOWN)
#define KPM_DEBUG_STATE_OFFSET     (UNKNOWN)

/* Bitfields (UNKNOWN when not available) */
#define KPM_MODE_MASK              (UNKNOWN)
#define KPM_MODE_RC_VAL            (UNKNOWN)
#define KPM_MODE_EP_VAL            (UNKNOWN)
#define KPM_LTSSM_EN_BIT           (UNKNOWN)
#define KPM_DEBUG_LTSSM_STATE_MASK (UNKNOWN)
#define KPM_DEBUG_STATE_L0_VAL     (UNKNOWN)

/* Error codes */
#define KPM_OK         (0)
#define KPM_EINVAL     (-1)
#define KPM_ENOTSUP    (-2)
#define KPM_EIO        (-3)
#define KPM_ETIME      (-4)

/* Endianness: big-endian MMIO helpers */
static inline uint32_t kpm_bswap32(uint32_t v) { return __builtin_bswap32(v); }
static inline uint32_t kpm_mmio_read32_be(uintptr_t addr)
{
    volatile uint32_t *p = (volatile uint32_t *)addr;
    uint32_t v = *p;
    return kpm_bswap32(v);
}
static inline void kpm_mmio_write32_be(uintptr_t addr, uint32_t v)
{
    volatile uint32_t *p = (volatile uint32_t *)addr;
    *p = kpm_bswap32(v);
}

/* Feature-specific API: Dual-mode operation (RC/EP) */

typedef enum {
    KPM_PCIE_MODE_RC = 0,
    KPM_PCIE_MODE_EP = 1,
} kpm_pcie_mode_t;

int kpm_dual_mode_init(void);
int kpm_dual_mode_configure(kpm_pcie_mode_t mode);
int kpm_dual_mode_get_mode(kpm_pcie_mode_t *mode);
int kpm_dual_mode_enable_ltssm(void);
int kpm_dual_mode_disable_ltssm(void);
int kpm_dual_mode_get_link_state(uint32_t *state_raw, bool *in_l0);

#ifdef __cplusplus
}
#endif

#endif /* KEYSTONE_PCIE_MODULE_DUAL_MODE_OPERATION_ROOT_COMPLEX_AND_END_POINT_H */
