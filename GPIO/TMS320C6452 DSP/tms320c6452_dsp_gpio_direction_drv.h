/*
 * tms320c6452_dsp_gpio_direction_drv.h
 * Single feature: GPIO direction control for TMS320C6452 DSP
 * Baremetal, gcc, big-endian device registers.
 */
#ifndef TMS320C6452_DSP_GPIO_DIRECTION_DRV_H
#define TMS320C6452_DSP_GPIO_DIRECTION_DRV_H

#include <stdint.h>
#include <stdbool.h>
#include "TMS320C6452 DSP_core.h"

/* Register aliases for this feature */
#define GD_DIR_ADDR            (GPIO_DIR_ADDR)
#define GD_OUT_ADDR            (GPIO_OUT_ADDR)
#define GD_IN_ADDR             (GPIO_IN_ADDR)
#define GD_SET_ADDR            (GPIO_SET_ADDR)
#define GD_CLR_ADDR            (GPIO_CLR_ADDR)

/* Bitfield helpers */
#define GD_PIN(bit)            GPIO_PIN(bit)
#define GD_MASK(bits)          (uint32_t)(bits)

/* API return codes */
#define GD_OK                  (0)
#define GD_EINVAL              (-1)

#ifdef __cplusplus
extern "C" {
#endif

/* Initialization for the GPIO direction feature */
int tms320c6452_dsp_gpio_direction_init(void);

/* Configure direction: set bits to output; clear bits to input */
int tms320c6452_dsp_gpio_direction_set_outputs(uint32_t mask);
int tms320c6452_dsp_gpio_direction_set_inputs(uint32_t mask);

/* Write outputs using atomic set/clear registers */
void tms320c6452_dsp_gpio_write_set(uint32_t mask);
void tms320c6452_dsp_gpio_write_clear(uint32_t mask);

/* Per-pin helpers */
int  tms320c6452_dsp_gpio_direction_set_pin(uint32_t bit, bool output);
int  tms320c6452_dsp_gpio_write_pin(uint32_t bit, bool value);
bool tms320c6452_dsp_gpio_read_pin(uint32_t bit);

/* Bulk readback */
uint32_t tms320c6452_dsp_gpio_read_inputs(void);
uint32_t tms320c6452_dsp_gpio_read_outputs(void);

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_DSP_GPIO_DIRECTION_DRV_H */
