#ifndef TMS320C6452_DSP_DIRECTION_CONTROL_DIR01_DRV_H
#define TMS320C6452_DSP_DIRECTION_CONTROL_DIR01_DRV_H

/*
 * Single-feature driver: Direction control (DIR01)
 * Controller: TMS320C6452 DSP
 * Target: arch=arm, env=uboot, compiler=gcc, endianness=bigendian
 * Constraints: Memory-mapped I/O, no dynamic allocation, deterministic access
 */

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Base address (from configuration) */
#define TMS320C6452_DSP_GPIO_BASE_DEFAULT   (0xA1008000u)

/* Register offset for this feature only */
#define TMS320C6452_DSP_GPIO_DIR01_OFFSET   (0x00000010u)

/* Bit helpers */
#define TMS320C6452_DSP_GPIO_DIR_PIN_MASK(pin)   ((uint32_t)1u << (uint32_t)(pin))

/* Error codes */
#define TMS320C6452_DSP_DIR_OK        (0)
#define TMS320C6452_DSP_DIR_EINVAL    (-22)
#define TMS320C6452_DSP_DIR_ESTATE    (-200)

/* Feature-local context (no dynamic memory) */
typedef struct {
    uintptr_t base;  /* MMIO base for GPIO */
    uint32_t  ready; /* 1 after init */
} tms320c6452_dsp_dir_ctx_t;

/* API: Initialize with base (use DEFAULT if base == 0) */
int tms320c6452_dsp_direction_control_dir01_init(tms320c6452_dsp_dir_ctx_t *ctx, uintptr_t base);

/* API: Deinitialize */
int tms320c6452_dsp_direction_control_dir01_deinit(tms320c6452_dsp_dir_ctx_t *ctx);

/* API: Configure pin as input (writes 1 to DIR01[pin]) */
int tms320c6452_dsp_direction_control_dir01_set_input(tms320c6452_dsp_dir_ctx_t *ctx, uint32_t pin);

/* API: Configure pin as output (writes 0 to DIR01[pin]) */
int tms320c6452_dsp_direction_control_dir01_set_output(tms320c6452_dsp_dir_ctx_t *ctx, uint32_t pin);

/* API: Read direction for a pin: *is_input = 1 for input, 0 for output */
int tms320c6452_dsp_direction_control_dir01_get(tms320c6452_dsp_dir_ctx_t *ctx, uint32_t pin, uint32_t *is_input);

/* API: Bulk write entire DIR01 mask (1=input, 0=output) */
int tms320c6452_dsp_direction_control_dir01_write_mask(tms320c6452_dsp_dir_ctx_t *ctx, uint32_t dir_mask);

/* API: Bulk read entire DIR01 mask */
int tms320c6452_dsp_direction_control_dir01_read_mask(tms320c6452_dsp_dir_ctx_t *ctx, uint32_t *dir_mask);

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_DSP_DIRECTION_CONTROL_DIR01_DRV_H */
