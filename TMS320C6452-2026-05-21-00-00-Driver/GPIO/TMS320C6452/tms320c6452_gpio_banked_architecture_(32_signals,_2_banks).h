#ifndef TMS320C6452_GPIO_BANKED_ARCHITECTURE_32_SIGNALS_2_BANKS_H
#define TMS320C6452_GPIO_BANKED_ARCHITECTURE_32_SIGNALS_2_BANKS_H

#include <stdint.h>
#include <stdbool.h>
#include "tms320c6452_gpio_core.h"

/* Feature: GPIO banked architecture (32 signals, 2 banks) */

/* API */

/* Initialize feature: all inputs, interrupts disabled, edges cleared */
int tms320c6452_gpio_banked_init(void);

/* Set direction mask across GP[31:0]: 1=input, 0=output */
void tms320c6452_gpio_banked_set_direction(uint32_t dir_mask);

/* Atomic output update: set_mask drives high, clr_mask drives low (for outputs) */
void tms320c6452_gpio_banked_write(uint32_t set_mask, uint32_t clr_mask);

/* Read synchronized input levels across GP[31:0] */
uint32_t tms320c6452_gpio_banked_read_inputs(void);

/* Configure per-pin edge detection */
void tms320c6452_gpio_banked_config_edge(uint32_t ris_enable_mask,
                                         uint32_t ris_disable_mask,
                                         uint32_t fal_enable_mask,
                                         uint32_t fal_disable_mask);

/* Read interrupt status bitmap across GP[31:0] */
uint32_t tms320c6452_gpio_banked_get_int_status(void);

/* ISR entry for banked GPIO; call from platform ISR */
void tms320c6452_gpio_banked_isr(void);

/* Weak callback invoked from ISR with pending bitmap; user may override */
void tms320c6452_gpio_banked_isr_callback(uint32_t pending_map) __attribute__((weak));

#endif /* TMS320C6452_GPIO_BANKED_ARCHITECTURE_32_SIGNALS_2_BANKS_H */
