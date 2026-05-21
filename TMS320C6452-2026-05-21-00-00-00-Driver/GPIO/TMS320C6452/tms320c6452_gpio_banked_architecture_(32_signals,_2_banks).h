#ifndef TMS320C6452_GPIO_BANKED_ARCHITECTURE_32_SIGNALS_2_BANKS_H
#define TMS320C6452_GPIO_BANKED_ARCHITECTURE_32_SIGNALS_2_BANKS_H

#include <stdint.h>
#include <stdbool.h>
#include "tms320c6452_gpio_core.h"

/* Single-feature driver: GPIO banked architecture (32 signals, 2 banks) */

/* Initialization: disables bank interrupts, clears all edge triggers, sets all pins as inputs */
int tms320c6452_gpio_banked_init(void);

/* Direction mask across GP[31:0]: 1=input, 0=output */
void tms320c6452_gpio_banked_set_direction(uint32_t dir_mask);

/* Atomic write: set high for bits in set_mask, drive low for bits in clr_mask (outputs only) */
void tms320c6452_gpio_banked_write(uint32_t set_mask, uint32_t clr_mask);

/* Read synchronized input levels for GP[31:0] */
uint32_t tms320c6452_gpio_banked_read_inputs(void);

/* Configure edge detection per pin: enable/disable masks for rising and falling edges */
void tms320c6452_gpio_banked_config_edge(uint32_t ris_enable_mask,
                                         uint32_t ris_disable_mask,
                                         uint32_t fal_enable_mask,
                                         uint32_t fal_disable_mask);

/* Enable/disable per-bank interrupt/event gating (bank 0 or 1) */
void tms320c6452_gpio_banked_enable_bank_irq(unsigned bank, int enable);

/* Read interrupt status bitmap across GP[31:0] */
uint32_t tms320c6452_gpio_banked_get_int_status(void);

/* ISR entry for banked GPIO. User should hook this to the appropriate CPU interrupt. */
void tms320c6452_gpio_banked_isr(void);

/* Weak callback invoked from ISR with pending bitmap */
void tms320c6452_gpio_banked_isr_callback(uint32_t pending_map) __attribute__((weak));

#endif /* TMS320C6452_GPIO_BANKED_ARCHITECTURE_32_SIGNALS_2_BANKS_H */
