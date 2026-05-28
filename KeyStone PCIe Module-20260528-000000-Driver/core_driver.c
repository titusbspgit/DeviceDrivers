/* Core driver implementation for KeyStone PCIe Module */
#include "core_driver.h"

/* Simple polls with bounded loops; return 0 on success, -1 on timeout */
static int _poll_bits_all_set(uint32_t off, uint32_t mask, uint32_t timeout_loops)
{
    while (timeout_loops--) {
        if ((pcie_read32(off) & mask) == mask)
            return 0;
    }
    return -1;
}

static int _poll_bits_all_clear(uint32_t off, uint32_t mask, uint32_t timeout_loops)
{
    while (timeout_loops--) {
        if ((pcie_read32(off) & mask) == 0)
            return 0;
    }
    return -1;
}

static const struct pcie_core_api core_api = {
    .read32 = pcie_read32,
    .write32 = pcie_write32,
    .set_bits = pcie_set_bits,
    .clr_bits = pcie_clr_bits,
    .delay = pcie_delay,
    .poll_bits_all_set = _poll_bits_all_set,
    .poll_bits_all_clear = _poll_bits_all_clear,
};

const struct pcie_core_api *pcie_core_get_api(void)
{
    return &core_api;
}
