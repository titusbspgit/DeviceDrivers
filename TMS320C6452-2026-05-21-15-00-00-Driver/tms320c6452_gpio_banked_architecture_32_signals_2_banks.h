#ifndef TMS320C6452_GPIO_BANKED_ARCHITECTURE_32_SIGNALS_2_BANKS_H
#define TMS320C6452_GPIO_BANKED_ARCHITECTURE_32_SIGNALS_2_BANKS_H

#include <stdint.h>
#include "tms320c6452_gpio_core.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TMS_GPIO_OK        (0)
#define TMS_GPIO_ERR_INVAL (-1)
#define TMS_GPIO_ERR_RANGE (-2)

typedef struct {
    const tms320c6452_gpio_core_t *core;
} tms320c6452_gpio_banked_architecture_32_signals_2_banks_handle_t;

int tms320c6452_gpio_banked_architecture_32_signals_2_banks_init(
    tms320c6452_gpio_banked_architecture_32_signals_2_banks_handle_t *h,
    const tms320c6452_gpio_core_t *core);

int tms320c6452_gpio_banked_architecture_32_signals_2_banks_set_direction(
    tms320c6452_gpio_banked_architecture_32_signals_2_banks_handle_t *h,
    uint32_t gpio,
    int is_input);

int tms320c6452_gpio_banked_architecture_32_signals_2_banks_write_pin(
    tms320c6452_gpio_banked_architecture_32_signals_2_banks_handle_t *h,
    uint32_t gpio,
    int value);

int tms320c6452_gpio_banked_architecture_32_signals_2_banks_read_pin(
    tms320c6452_gpio_banked_architecture_32_signals_2_banks_handle_t *h,
    uint32_t gpio,
    int *value);

int tms320c6452_gpio_banked_architecture_32_signals_2_banks_enable_bank_interrupt(
    tms320c6452_gpio_banked_architecture_32_signals_2_banks_handle_t *h,
    uint32_t bank,
    int enable);

int tms320c6452_gpio_banked_architecture_32_signals_2_banks_configure_pin_interrupt(
    tms320c6452_gpio_banked_architecture_32_signals_2_banks_handle_t *h,
    uint32_t gpio,
    int rising,
    int falling);

uint32_t tms320c6452_gpio_banked_architecture_32_signals_2_banks_get_int_status(
    tms320c6452_gpio_banked_architecture_32_signals_2_banks_handle_t *h);

void tms320c6452_gpio_banked_architecture_32_signals_2_banks_clear_int_status(
    tms320c6452_gpio_banked_architecture_32_signals_2_banks_handle_t *h,
    uint32_t mask);

#ifdef __cplusplus
}
#endif

#endif
