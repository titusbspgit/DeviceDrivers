/* Core driver implementation for TMS320C6452 DSP (big-endian aware) */
#include "tms320c6452_dsp_core.h"

static uint32_t core_read32_adapter(volatile void const *addr)
{
    return core_mmio_read32_be(addr);
}

static void core_write32_adapter(volatile void *addr, uint32_t v)
{
    core_mmio_write32_be(addr, v);
}

static void core_set_bits32_adapter(volatile void *addr, uint32_t m)
{
    core_mmio_set_bits32_be(addr, m);
}

static void core_clear_bits32_adapter(volatile void *addr, uint32_t m)
{
    core_mmio_clear_bits32_be(addr, m);
}

static void core_delay_cycles_adapter(volatile uint32_t cycles)
{
    core_delay_cycles(cycles);
}

static void core_dmb_adapter(void) { core_dmb(); }
static void core_dsb_adapter(void) { core_dsb(); }
static void core_isb_adapter(void) { core_isb(); }

const tms320c6452_dsp_core_ops_t tms320c6452_dsp_core_ops = {
    .read32       = core_read32_adapter,
    .write32      = core_write32_adapter,
    .set_bits32   = core_set_bits32_adapter,
    .clear_bits32 = core_clear_bits32_adapter,
    .delay_cycles = core_delay_cycles_adapter,
    .dmb          = core_dmb_adapter,
    .dsb          = core_dsb_adapter,
    .isb          = core_isb_adapter,
};
