#ifndef KEYSTONE_PCIE_MODULE_LINK_WIDTH_AND_SPEED_X1_X2_GEN1_GEN2_H
#define KEYSTONE_PCIE_MODULE_LINK_WIDTH_AND_SPEED_X1_X2_GEN1_GEN2_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "core_KeyStone_PCIe_Module.h"

/* Registers and bitfields (unknown => not supported here) */
#define KPM_LINK_CTRL_OFFSET          (KPM_REG_UNKNOWN)
#define KPM_LINK_CTRL2_OFFSET         (KPM_REG_UNKNOWN)
#define KPM_LINK_STAT_OFFSET          (KPM_REG_UNKNOWN)

#define KPM_LINK_CTRL_WIDTH_MASK      (KPM_REG_UNKNOWN)
#define KPM_LINK_CTRL_WIDTH_X1        (KPM_REG_UNKNOWN)
#define KPM_LINK_CTRL_WIDTH_X2        (KPM_REG_UNKNOWN)

#define KPM_LINK_CTRL2_SPEED_MASK     (KPM_REG_UNKNOWN)
#define KPM_LINK_CTRL2_SPEED_GEN1     (KPM_REG_UNKNOWN)
#define KPM_LINK_CTRL2_SPEED_GEN2     (KPM_REG_UNKNOWN)

#define KPM_LINK_STAT_NEG_WIDTH_MASK  (KPM_REG_UNKNOWN)
#define KPM_LINK_STAT_NEG_WIDTH_X1    (KPM_REG_UNKNOWN)
#define KPM_LINK_STAT_NEG_WIDTH_X2    (KPM_REG_UNKNOWN)
#define KPM_LINK_STAT_SPEED_MASK      (KPM_REG_UNKNOWN)
#define KPM_LINK_STAT_SPEED_GEN1      (KPM_REG_UNKNOWN)
#define KPM_LINK_STAT_SPEED_GEN2      (KPM_REG_UNKNOWN)

typedef enum {
    KPM_LINK_WIDTH_X1 = 1,
    KPM_LINK_WIDTH_X2 = 2,
} kpm_link_width_t;

typedef enum {
    KPM_LINK_SPEED_GEN1 = 1,
    KPM_LINK_SPEED_GEN2 = 2,
} kpm_link_speed_t;

int kpm_linkws_init(const kpm_core_api_t *core);
int kpm_linkws_set_advertised(const kpm_core_api_t *core, kpm_link_width_t width, kpm_link_speed_t speed);
int kpm_linkws_get_negotiated(const kpm_core_api_t *core, kpm_link_width_t *width, kpm_link_speed_t *speed);

#ifdef __cplusplus
}
#endif

#endif /* KEYSTONE_PCIE_MODULE_LINK_WIDTH_AND_SPEED_X1_X2_GEN1_GEN2_H */
