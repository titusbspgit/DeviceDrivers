// Baremetal core access implementation for TMS320C6452 DSP
// Environment: arch=arm, compiler=gcc, endianness=bigendian

#include "TMS320C6452_DSP_core.h"

static uint8_t  _rd8 (uintptr_t a) { return tms320c6452_dsp_read8(a); }
static uint16_t _rd16(uintptr_t a) { return tms320c6452_dsp_read16_be(a); }
static uint32_t _rd32(uintptr_t a) { return tms320c6452_dsp_read32_be(a); }
static void     _wr8 (uintptr_t a, uint8_t  v) { tms320c6452_dsp_write8(a, v); }
static void     _wr16(uintptr_t a, uint16_t v) { tms320c6452_dsp_write16_be(a, v); }
static void     _wr32(uintptr_t a, uint32_t v) { tms320c6452_dsp_write32_be(a, v); }

static const TMS320C6452_DSP_CoreAPI _core = {
    .base = TMS320C6452_DSP_BASE_ADDR,
    .rd8  = _rd8,
    .rd16 = _rd16,
    .rd32 = _rd32,
    .wr8  = _wr8,
    .wr16 = _wr16,
    .wr32 = _wr32,
};

const TMS320C6452_DSP_CoreAPI *TMS320C6452_DSP_core_get(void)
{
    return &_core;
}
