#ifndef TMS320C6452_DIRECTION_CONTROL_DIR01_H
#define TMS320C6452_DIRECTION_CONTROL_DIR01_H

#include <stdint.h>
#include <stdbool.h>
#include "tms320c6452_gpio_core.h"

/* Feature: Direction control (DIR01) */

/* Initialize: set all GPIOs as inputs (reset state) */
int tms320c6452_direction_control_init(void);

/* Write full direction mask: 1=input, 0=output */
void tms320c6452_direction_set_mask(uint32_t dir_mask);

/* Read back full direction mask */
uint32_t tms320c6452_direction_get_mask(void);

/* Configure single pin direction: is_input!=0 -> input, else output */
int tms320c6452_direction_set_pin(unsigned pin, int is_input);

#endif /* TMS320C6452_DIRECTION_CONTROL_DIR01_H */
