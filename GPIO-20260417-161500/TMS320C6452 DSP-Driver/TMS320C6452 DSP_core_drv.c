/*
 * Core Driver Source for TMS320C6452 DSP
 * Environment: U-Boot, gcc, big-endian
 * Provides reusable MMIO helpers and function table for feature drivers.
 */
#include "TMS320C6452 DSP_core_drv.h"

/* Static ops table mapping to the inline helpers */
static const tms320c6452_core_ops_t g_tms320c6452_core_ops = {
    .read8         = core_read8,
    .write8        = core_write8,
    .read16        = core_read16,
    .write16       = core_write16,
    .read32        = core_read32,
    .write32       = core_write32,
    .set_bits32    = core_set_bits32,
    .clear_bits32  = core_clear_bits32,
    .update_bits32 = core_update_bits32,
};

const tms320c6452_core_ops_t *tms320c6452_core_get_ops(void)
{
    return &g_tms320c6452_core_ops;
}
