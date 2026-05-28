#ifndef KEYSTONE_PCIE_MODULE_LINK_WIDTH_AND_SPEED_X1_X2_GEN1_GEN2_H
#define KEYSTONE_PCIE_MODULE_LINK_WIDTH_AND_SPEED_X1_X2_GEN1_GEN2_H

#include <stdint.h>
#include <stdbool.h>
#include "core_keystone_pcie_module.h"

/* Registers and bitfields for Link width/speed (UNKNOWN if not provided) */
#define KPM_REG_LINK_CTRL_OFFSET          0xFFFFFFFFu /* UNKNOWN */
#define KPM_REG_LINK_STATUS_OFFSET        0xFFFFFFFFu /* UNKNOWN */
#define KPM_LINK_CTRL_TARGET_SPEED_MASK   0x0000000Fu /* UNKNOWN */
#define KPM_LINK_CTRL_TARGET_SPEED_SHIFT  0u          /* UNKNOWN */
#define KPM_LINK_CTRL_WIDTH_MASK          0x000000F0u /* UNKNOWN */
#define KPM_LINK_CTRL_WIDTH_SHIFT         4u          /* UNKNOWN */
#define KPM_LINK_STATUS_SPEED_MASK        0x0000000Fu /* UNKNOWN */
#define KPM_LINK_STATUS_SPEED_SHIFT       0u          /* UNKNOWN */
#define KPM_LINK_STATUS_WIDTH_MASK        0x000000F0u /* UNKNOWN */
#define KPM_LINK_STATUS_WIDTH_SHIFT       4u          /* UNKNOWN */

/* Return codes */
#define KPM_OK                 (0)
#define KPM_ERR_INVAL          (-1)
#define KPM_ERR_UNSUPPORTED    (-2)

typedef enum {
    KPM_SPEED_GEN1 = 1,
    KPM_SPEED_GEN2 = 2
} KPM_LinkSpeed;

typedef enum {
    KPM_WIDTH_X1 = 1,
    KPM_WIDTH_X2 = 2
} KPM_LinkWidth;

#ifdef __cplusplus
extern "C" {
#endif

int kpm_link_init(const KPM_CoreApi *api);
int kpm_link_request(const KPM_CoreApi *api, KPM_LinkWidth width, KPM_LinkSpeed speed);
int kpm_link_query(const KPM_CoreApi *api, KPM_LinkWidth *width, KPM_LinkSpeed *speed);

#ifdef __cplusplus
}
#endif

#endif /* KEYSTONE_PCIE_MODULE_LINK_WIDTH_AND_SPEED_X1_X2_GEN1_GEN2_H */
