/* tms320c6452_dsp_per_pin_direct_interrupt_option_drv.c - Per-pin direct interrupt option (SINGLE FEATURE)
 * Registers/bitfields: UNKNOWN (device-specific). This implementation is a deterministic stub.
 */
#include "tms320c6452_dsp_per_pin_direct_interrupt_option.h"

static const core_tms320c6452_ops_t *g_ops;

int tms320c6452_dsp_per_pin_direct_interrupt_option_init(const core_tms320c6452_ops_t *ops) {
    if (!ops) return DRV_INVALID_ARG;
    g_ops = ops;
    /* No registers to program because mapping data is UNKNOWN. */
    return DRV_OK;
}

int tms320c6452_dsp_per_pin_direct_interrupt_option_map(uint32_t pin, uint32_t cpu_int) {
    (void)pin; (void)cpu_int;
    /* Mapping registers are UNKNOWN per CSV; feature not supported in this build. */
    return DRV_UNSUPPORTED;
}

int tms320c6452_dsp_per_pin_direct_interrupt_option_enable(uint32_t pin, bool enable) {
    (void)pin; (void)enable;
    /* Enable path is UNKNOWN without documented mapping registers. */
    return DRV_UNSUPPORTED;
}
