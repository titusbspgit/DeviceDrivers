/* SPDX-License-Identifier: MIT */
/* File: drivers/tms320c6452/gpio/gpio.h */
/* RCI-ag-DrvGen Agent - TMS320C6452 DSP GPIO Driver (Big-endian, Baremetal, GCC) */
#ifndef TMS320C6452_GPIO_H
#define TMS320C6452_GPIO_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "../hal/reg_access.h"
#include "gpio_regs.h"

/**
 * @file gpio.h
 * @brief GPIO driver public API for TI TMS320C6452 DSP.
 */

typedef enum {
    TMSGPIO_OK = 0,
    TMSGPIO_ERR_INVAL = -1,
    TMSGPIO_ERR_UNSUPPORTED = -2
} tmsgpio_status_t;

typedef enum {
    TMSGPIO_DIR_INPUT = 1,  /* per spec: 1=input */
    TMSGPIO_DIR_OUTPUT = 0
} tmsgpio_dir_t;

typedef struct {
    uintptr_t base;  /* MMIO base address */
} tmsgpio_t;

/* Platform hooks (weak) for PSC and Pinmux */
void tmsgpio_platform_enable_psc(void);
void tmsgpio_platform_disable_psc(void);
void tmsgpio_platform_configure_pinmux(void);

/* Initialize driver context. If base==0, uses TMS320C6452_GPIO_BASE_DEFAULT. */
int tmsgpio_init(tmsgpio_t *ctx, uintptr_t base, bool perform_platform_init);

/* Read PID register */
int tmsgpio_get_pid(const tmsgpio_t *ctx, uint32_t *out_pid);

/* Configure single pin direction */
int tmsgpio_config_pin(const tmsgpio_t *ctx, uint32_t pin, tmsgpio_dir_t dir);

/* Configure mask: 1=input, 0=output */
int tmsgpio_config_mask(const tmsgpio_t *ctx, uint32_t dir_mask);

/* Output control */
int tmsgpio_write_pin(const tmsgpio_t *ctx, uint32_t pin, bool high);
int tmsgpio_set_mask(const tmsgpio_t *ctx, uint32_t mask);
int tmsgpio_clear_mask(const tmsgpio_t *ctx, uint32_t mask);

/* Readbacks */
int tmsgpio_read_pin(const tmsgpio_t *ctx, uint32_t pin, bool *level);
int tmsgpio_read_port(const tmsgpio_t *ctx, uint32_t *port_bits);

/* Interrupts */
int tmsgpio_irq_config_edge(const tmsgpio_t *ctx, uint32_t pin, bool rising_enable, bool falling_enable);
int tmsgpio_irq_bank_enable(const tmsgpio_t *ctx, uint32_t bank, bool enable);
int tmsgpio_irq_status(const tmsgpio_t *ctx, uint32_t *status_bits);
int tmsgpio_irq_clear(const tmsgpio_t *ctx, uint32_t mask);

/* Low power helpers */
int tmsgpio_enter_low_power(const tmsgpio_t *ctx);
int tmsgpio_exit_low_power(const tmsgpio_t *ctx);

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_GPIO_H */
