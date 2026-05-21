/* core_TMS320C6452_DSP.c - Core MMIO helpers for TMS320C6452 DSP */
#include "core_TMS320C6452_DSP.h"

static const core_tms320c6452_ops_t s_ops = {
    .write8  = core_write8,
    .write16 = core_write16,
    .write32 = core_write32,
    .read8   = core_read8,
    .read16  = core_read16,
    .read32  = core_read32,
    .dmb     = core_dmb,
    .dsb     = core_dsb,
    .isb     = core_isb,
    .base    = TMS320C6452_DSP_BASE_ADDR,
};

const core_tms320c6452_ops_t *core_tms320c6452_get_ops(void) {
    return &s_ops;
}
