/*
 * Feature: Direction control (DIR01)
 * Controller: TMS320C6452 DSP
 *
 * Single-feature header. Depends on TMS320C6452 DSP_core.h
 */
#ifndef TMS320C6452_DSP_DIRECTION_CONTROL_DIR01_H
#define TMS320C6452_DSP_DIRECTION_CONTROL_DIR01_H

#include <stdint.h>
#include "TMS320C6452 DSP_core.h"

#ifdef __cplusplus
extern "C" {
#endif

/* DIR01 register name is known; the offset is not provided: mark UNKNOWN */
#define GPIO_DIR01_OFFSET_UNKNOWN 1

/* APIs specific to this feature */

gpio_drv_status_t gpio_dir_init(void);

gpio_drv_status_t gpio_set_pin_output(uint32_t pin);

gpio_drv_status_t gpio_set_pin_input(uint32_t pin);

gpio_drv_status_t gpio_get_pin_is_input(uint32_t pin, uint32_t *is_input);

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_DSP_DIRECTION_CONTROL_DIR01_H */
