#ifndef TMS320C6452_DSP_DIRECTION_CONTROL_DIR01_DRV_H
#define TMS320C6452_DSP_DIRECTION_CONTROL_DIR01_DRV_H

/*
 * Single-feature driver: Direction control (DIR01)
 * Target: arch=arm, env=uboot, compiler=gcc, endianness=bigendian
 * Constraints: MMIO, no dynamic allocation, deterministic register access
 */

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Base address (default from input contract). Can be overridden in init(). */
#define TMS320C6452_DSP_GPIO_BASE_DEFAULT   (0xA1008000u)

/* Register offsets (relative to GPIO base). Source: RAG */
#define TMS320C6452_DSP_GPIO_DIR01_OFFSET   (0x0010u)

/* Register access macro */
#define TMS320C6452_DSP_GPIO_REG(base, off) (*((volatile uint32_t *)((uintptr_t)(base) + (uint32_t)(off))))

/* Error codes */
typedef enum {
    TMS320C6452_DSP_DIR_OK = 0,
    TMS320C6452_DSP_DIR_EINVAL = -22, /* Invalid argument */
    TMS320C6452_DSP_DIR_ESTATE = -200, /* Invalid/uninitialized state */
} tms320c6452_dsp_dir_err_t;

/* Driver context for this single feature */
typedef struct {
    uintptr_t base;    /* MMIO base for GPIO */
    uint32_t  ready;   /* 1 after init */
} tms320c6452_dsp_dir_ctx_t;

/* API: Initialize feature driver with a base address (use DEFAULT if 0) */
int tms320c6452_dsp_direction_control_dir01_init(tms320c6452_dsp_dir_ctx_t *ctx, uintptr_t base);

/* API: Deinitialize feature driver */
int tms320c6452_dsp_direction_control_dir01_deinit(tms320c6452_dsp_dir_ctx_t *ctx);

/* API: Set pin as input (1 in DIR01) */
int tms320c6452_dsp_direction_control_dir01_set_input(tms320c6452_dsp_dir_ctx_t *ctx, uint32_t pin);

/* API: Set pin as output (0 in DIR01) */
int tms320c6452_dsp_direction_control_dir01_set_output(tms320c6452_dsp_dir_ctx_t *ctx, uint32_t pin);

/* API: Read direction bit for a pin: *is_input = 1 for input, 0 for output */
int tms320c6452_dsp_direction_control_dir01_get(tms320c6452_dsp_dir_ctx_t *ctx, uint32_t pin, uint32_t *is_input);

/* API: Bulk write entire DIR01 mask (1=input, 0=output) */
int tms320c6452_dsp_direction_control_dir01_write_mask(tms320c6452_dsp_dir_ctx_t *ctx, uint32_t dir_mask);

/* API: Bulk read entire DIR01 mask */
int tms320c6452_dsp_direction_control_dir01_read_mask(tms320c6452_dsp_dir_ctx_t *ctx, uint32_t *dir_mask);

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_DSP_DIRECTION_CONTROL_DIR01_DRV_H */
