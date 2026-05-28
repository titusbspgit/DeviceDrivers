/* Driver: MSI reception (RC mode) */
#include "keystone_pcie_module_msi_reception_rc_mode.h"

static inline uint32_t _msi_grp_base(uint32_t vec)
{
    uint32_t grp = (vec >> 2) & 0x7u; /* 0..7, 4 vectors per group if 32 total */
    return MSI_GROUP_BASE0_OFF + grp * MSI_GROUP_STRIDE;
}

static inline uint32_t _msi_bit(uint32_t vec)
{ return 1u << (vec & 0x1Fu); }

int kpm_msi_enable_vector(const struct pcie_core_api *api, uint32_t vec)
{
    uint32_t base = _msi_grp_base(vec);
    api->write32(base + MSI_ENABLE_SET_OFF, _msi_bit(vec));
    return 0;
}

int kpm_msi_disable_vector(const struct pcie_core_api *api, uint32_t vec)
{
    uint32_t base = _msi_grp_base(vec);
    api->write32(base + MSI_ENABLE_CLR_OFF, _msi_bit(vec));
    return 0;
}

int kpm_msi_poll_and_clear(const struct pcie_core_api *api, uint32_t vec, bool *active)
{
    uint32_t base = _msi_grp_base(vec);
    uint32_t st = api->read32(base + MSI_STATUS_OFF);
    uint32_t bit = _msi_bit(vec);
    *active = (st & bit) != 0u;
    if (*active) {
        /* W1C to STATUS to clear */
        api->write32(base + MSI_STATUS_OFF, bit);
    }
    return 0;
}
