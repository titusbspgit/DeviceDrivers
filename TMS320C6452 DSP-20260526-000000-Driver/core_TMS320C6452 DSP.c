/*
 * core_TMS320C6452 DSP.c
 * Reusable core access layer for TMS320C6452 DSP peripherals
 * Big-endian, bare-metal, GCC
 */
#include "core_TMS320C6452 DSP.h"

/* Internal init/deinit stubs (no HW side-effects here) */
static int core_tms320c6452_dsp_init_impl(void)
{
    /* Deterministic no-op; extend if global clocks/gates are required */
    return 0;
}

static void core_tms320c6452_dsp_deinit_impl(void)
{
    /* Deterministic no-op */
}

/* Singleton API instance mapping to inline accessors */
static const core_tms320c6452_dsp_api_t g_core_api = {
    .base   = CORE_TMS320C6452_DSP_BASE,
    .write8 = core_tms320c6452_dsp_write8,
    .read8  = core_tms320c6452_dsp_read8,
    .write16= core_tms320c6452_dsp_write16,
    .read16 = core_tms320c6452_dsp_read16,
    .write32= core_tms320c6452_dsp_write32,
    .read32 = core_tms320c6452_dsp_read32,
    .init   = core_tms320c6452_dsp_init_impl,
    .deinit = core_tms320c6452_dsp_deinit_impl
};

const core_tms320c6452_dsp_api_t* core_tms320c6452_dsp_get_api(void)
{
    return &g_core_api;
}
