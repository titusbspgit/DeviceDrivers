#ifndef TMS320C6452_DSP_WRITE_OUTPUT_OUT_DATA01_H
#define TMS320C6452_DSP_WRITE_OUTPUT_OUT_DATA01_H

/*
 * Feature: Write output (OUT_DATA01)
 * Controller: TMS320C6452 DSP
 * Environment: arch=arm, env=uboot, compiler=gcc, endianness=bigendian
 * Constraints: MMIO, no dynamic allocation, deterministic register access
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Base address (MMIO) */
#define TMS320C6452_DSP_GPIO_BASE_DEFAULT   (0xA1008000u)

/* Register offsets (relative to GPIO base) */
#define TMS320C6452_DSP_GPIO_OUT_DATA01_OFFSET   (0x0014u)

/* Register accessor macro (typed) */
#define TMS320C6452_DSP_GPIO_REG32(base, off) (*((volatile uint32_t *)((uintptr_t)(base) + (uint32_t)(off))))

/* Bit helpers */
#define TMS320C6452_DSP_GPIO_PIN_MASK(pin)   ( (uint32_t)1u << (uint32_t)(pin) )

/* Error codes */
#define TMS320C6452_DSP_OUT_OK        (0)
#define TMS320C6452_DSP_OUT_EINVAL    (-22)
#define TMS320C6452_DSP_OUT_ESTATE    (-200)

/* Driver context (feature-local) */
typedef struct {
    uintptr_t base;   /* MMIO base for GPIO */
    uint32_t  ready;  /* 1 after init */
} tms320c6452_dsp_out_ctx_t;

/* API: Initialize with a base address (use DEFAULT if base==0) */
int tms320c6452_dsp_write_output_out_data01_init(tms320c6452_dsp_out_ctx_t *ctx, uintptr_t base);

/* API: Deinitialize */
int tms320c6452_dsp_write_output_out_data01_deinit(tms320c6452_dsp_out_ctx_t *ctx);

/* API: Write a single pin output value (0 or 1) using RMW on OUT_DATA01 */
int tms320c6452_dsp_write_output_out_data01_write_pin(tms320c6452_dsp_out_ctx_t *ctx, uint32_t pin, uint32_t value);

/* API: Read a single pin output value as reflected in OUT_DATA01 */
int tms320c6452_dsp_write_output_out_data01_read_pin(tms320c6452_dsp_out_ctx_t *ctx, uint32_t pin, uint32_t *value);

/* API: Bulk write full OUT_DATA01 register */
int tms320c6452_dsp_write_output_out_data01_write_all(tms320c6452_dsp_out_ctx_t *ctx, uint32_t out_value);

/* API: Bulk read full OUT_DATA01 register */
int tms320c6452_dsp_write_output_out_data01_read_all(tms320c6452_dsp_out_ctx_t *ctx, uint32_t *out_value);

/* API: Masked update: for each bit set in mask, write the corresponding bit from value into OUT_DATA01 */
int tms320c6452_dsp_write_output_out_data01_update_mask(tms320c6452_dsp_out_ctx_t *ctx, uint32_t mask, uint32_t value);

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_DSP_WRITE_OUTPUT_OUT_DATA01_H */
