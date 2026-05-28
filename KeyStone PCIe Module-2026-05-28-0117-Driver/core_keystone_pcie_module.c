/* Core driver for KeyStone PCIe Module - Baremetal, ARM, GCC, Big-endian CPU */
#include "core_keystone_pcie_module.h"

#ifndef KS_PCIE_SPIN_DELAY_CYCLES_PER_US
#define KS_PCIE_SPIN_DELAY_CYCLES_PER_US  50u
#endif

/* Optional weak hook to provide precise microsecond delays on target */
__attribute__((weak)) void ks_pcie_udelay(uint32_t usec)
{
    /* Fallback busy-wait loop (approximate) */
    volatile uint32_t i;
    while (usec--) {
        for (i = 0; i < KS_PCIE_SPIN_DELAY_CYCLES_PER_US; ++i) {
            __asm__ volatile("");
        }
    }
}

static uint32_t _read32(uint32_t offset)
{
    ks_pcie_dmb();
    uint32_t v = KS_FROM_REG32(KS_PCIE_REG32(offset));
    ks_pcie_dmb();
    return v;
}

static void _write32(uint32_t offset, uint32_t value)
{
    ks_pcie_dmb();
    KS_PCIE_REG32(offset) = KS_TO_REG32(value);
    ks_pcie_dmb();
}

static void _set_bits(uint32_t offset, uint32_t mask)
{
    uint32_t v = _read32(offset);
    v |= mask;
    _write32(offset, v);
}

static void _clear_bits(uint32_t offset, uint32_t mask)
{
    uint32_t v = _read32(offset);
    v &= ~mask;
    _write32(offset, v);
}

static int _poll32(uint32_t offset, uint32_t mask, uint32_t value, uint32_t timeout_us)
{
    uint32_t elapsed = 0;
    for (;;) {
        uint32_t v = _read32(offset);
        if ((v & mask) == (value & mask)) {
            return 0;
        }
        if (elapsed >= timeout_us) {
            return -1;
        }
        ks_pcie_udelay(1);
        elapsed += 1;
    }
}

static const ks_pcie_core_api_t g_api = {
    .read32    = _read32,
    .write32   = _write32,
    .set_bits  = _set_bits,
    .clear_bits= _clear_bits,
    .poll32    = _poll32,
};

void ks_pcie_core_init(void)
{
    /* Nothing persistent to initialize currently; keep for symmetry and future hooks */
    ks_pcie_dmb();
}

const ks_pcie_core_api_t * ks_pcie_core(void)
{
    return &g_api;
}

/* Direct wrappers */
uint32_t ks_pcie_read32(uint32_t offset)                  { return g_api.read32(offset); }
void     ks_pcie_write32(uint32_t offset, uint32_t value) { g_api.write32(offset, value); }
void     ks_pcie_set_bits(uint32_t offset, uint32_t mask) { g_api.set_bits(offset, mask); }
void     ks_pcie_clear_bits(uint32_t offset, uint32_t mask){ g_api.clear_bits(offset, mask);} 
int      ks_pcie_poll32(uint32_t offset, uint32_t mask, uint32_t value, uint32_t timeout_us)
{
    return g_api.poll32(offset, mask, value, timeout_us);
}
