#ifndef CORE_KEYSTONE_PCIE_MODULE_H
#define CORE_KEYSTONE_PCIE_MODULE_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Base address (big-endian system mapping) */
#define PCIE_BASE_ADDR              ((uintptr_t)0xA1008000u)

/* Representative register offsets (placeholders where SoC-specific) */
#define PCIE_CMD_STATUS_OFF         (0x0004u)
#define PCIE_LINK_CTRL2_OFF         (0x00A0u) /* placeholder */
#define PCIE_LINK_STAT_CTRL_OFF     (0x0100u) /* placeholder */
#define PCIE_IRQ_EOI_OFF            (0x0050u) /* placeholder */
#define PCIE_MSI_IRQ_STATUS_OFF     (0x0054u) /* placeholder */
#define PCIE_MSI_EN_SET_G0_OFF      (0x0090u) /* placeholder */
#define PCIE_MSI_EN_CLR_G0_OFF      (0x0094u) /* placeholder */
#define PCIE_PM_CAP_OFF             (0x1040u) /* placeholder */
#define PCIE_AER_CAP_OFF            (0x1300u) /* placeholder */

/* Bit helpers (subset, placeholders) */
#define CMD_STATUS_LTSSM_EN         (1u << 0)
#define LINK_STAT_CTRL_COMMON_CLK   (1u << 6)
#define LINK_CTRL2_TARGET_GEN2      (0x2u)

/* Endian helpers for big-endian baremetal target */
#ifndef cpu_to_be32
#define cpu_to_be32(x)              (uint32_t)(x)
#endif
#ifndef be32_to_cpu
#define be32_to_cpu(x)              (uint32_t)(x)
#endif

static inline void pcie_mb(void) { __asm__ __volatile__("" ::: "memory"); }
static inline volatile uint32_t* pcie_reg(uint32_t off)
{
    return (volatile uint32_t*)(PCIE_BASE_ADDR + (uintptr_t)off);
}
static inline uint32_t pcie_read32(uint32_t off)
{
    uint32_t v = *pcie_reg(off);
    pcie_mb();
    return be32_to_cpu(v);
}
static inline void pcie_write32(uint32_t off, uint32_t val)
{
    *pcie_reg(off) = cpu_to_be32(val);
    pcie_mb();
}

/* Core API exposed to feature drivers */
typedef struct pcie_core_ops {
    int  (*init)(void);
    int  (*enable_ltssm)(bool en);
    uint32_t (*get_ltssm_state)(void);
    int  (*set_link_speed_gen2)(bool en);
    int  (*set_common_clock)(bool en);
    int  (*msi_enable_group)(unsigned group, bool en);
    int  (*msi_ack)(unsigned vector);
    int  (*endianness_set)(bool big_endian);
    uint32_t (*reg_read)(uint32_t off);
    void (*reg_write)(uint32_t off, uint32_t val);
} pcie_core_ops_t;

/* Get handle to default ops (implemented in core_keystone_pcie_module.c) */
const pcie_core_ops_t* pcie_core_get_ops(void);

#endif /* CORE_KEYSTONE_PCIE_MODULE_H */
