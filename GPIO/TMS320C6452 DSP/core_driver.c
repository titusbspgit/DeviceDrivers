#include "core_driver.h"

static uint32_t _read32(uint32_t off)              { return core_read32(off); }
static void     _write32(uint32_t off, uint32_t v) { core_write32(off, v); }
static void     _set_bits(uint32_t off, uint32_t m){ core_set_bits(off, m); }
static void     _clr_bits(uint32_t off, uint32_t m){ core_clr_bits(off, m); }
static int      _poll(uint32_t off, uint32_t mask, uint32_t exp, uint32_t to)
{
    return core_poll_bits(off, mask, exp, to);
}

static const drv_core_api_t g_core_api = {
    .read32    = _read32,
    .write32   = _write32,
    .set_bits  = _set_bits,
    .clr_bits  = _clr_bits,
    .poll_bits = _poll
};

const drv_core_api_t* drv_core(void)
{
    return &g_core_api;
}
