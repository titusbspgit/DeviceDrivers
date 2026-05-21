/* Feature: GPIO direction control (DIR01) */
#ifndef TMS320C6452_DSP_GPIO_DIRECTION_CONTROL_DIR01_H
#define TMS320C6452_DSP_GPIO_DIRECTION_CONTROL_DIR01_H

#include <stdint.h>
#include "core_TMS320C6452 DSP.h"

/* Register mapping (from spec): DIR01 at offset 0x10 from base */
#define DIR01_ADDR   GPIO_DIR01_ADDR

/* Bit helper for GP[31:0] */
#define DIR01_BIT(n) ( (uint32_t)(1u << ((n) & 31u)) )

#ifdef __cplusplus
extern "C" {
#endif

/* Initialize feature driver with core API */
int tms320c6452_dsp_gpio_direction_control_dir01_init(const core_api_t* api);

/* Raw read/write of DIR01 register */
uint32_t tms320c6452_dsp_gpio_direction_control_dir01_read(void);
int      tms320c6452_dsp_gpio_direction_control_dir01_write(uint32_t value);

/* Masked modification helpers (semantics of 1=input or 1=output are device-specific/UNKNOWN) */
int tms320c6452_dsp_gpio_direction_control_dir01_set_bits(uint32_t mask);
int tms320c6452_dsp_gpio_direction_control_dir01_clear_bits(uint32_t mask);

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_DSP_GPIO_DIRECTION_CONTROL_DIR01_H */
