#include "core_keystone_pcie_module.h"

static const kpm_core_api_t kpm_core_default = {
    .base = KPM_PCIE_BASE,
    .read32 = kpm_read32,
    .write32 = kpm_write32,
    .mb = kpm_mb,
    .udelay = kpm_udelay,
};

const kpm_core_api_t *kpm_core_get_default(void)
{
    return &kpm_core_default;
}
