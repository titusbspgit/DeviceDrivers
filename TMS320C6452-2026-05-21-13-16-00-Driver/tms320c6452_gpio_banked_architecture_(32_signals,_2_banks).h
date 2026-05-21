#ifndef TMS320C6452_GPIO_BANKED_ARCHITECTURE_32_SIGNALS_2_BANKS_H
#define TMS320C6452_GPIO_BANKED_ARCHITECTURE_32_SIGNALS_2_BANKS_H

#include <stdint.h>
#include "tms320c6452_gpio_core.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Error codes */
#define TMS_GPIO_OK                 (0)
#define TMS_GPIO_ERR_INVAL          (-1)
#define TMS_GPIO_ERR_RANGE          (-2)

/* Handle for this single feature driver */
typedef struct {
    const tms320c6452_gpio_core_t *core; /* must be non-NULL */
} tms320c6452_gpio_banked_architecture_32_signals_2_banks_handle_t;

/* API declarations (single feature only) */
int tms320c6452_gpio_banked_architecture_32_signals_2_banks_init(
    tms320c6452_gpio_banked_architecture_32_signals_2_banks_handle_t *h,
    const tms320c6452_gpio_core_t *core);

int tms320c6452_gpio_banked_architecture_32_signals_2_banks_set_direction(
    tms320c6452_gpio_banked_architecture_32_signals_2_banks_handle_t *h,
    uint32_t gpio, /* 0..31 */
    int is_input /* 0=output, 1=input */);

int tms320c6452_gpio_banked_architecture_32_signals_2_banks_write_pin(
    tms320c6452_gpio_banked_architecture_32_signals_2_banks_handle_t *h,
    uint32_t gpio, /* 0..31 */
    int value /* 0/1 */);

int tms320c6452_gpio_banked_architecture_32_signals_2_banks_read_pin(
    tms320c6452_gpio_banked_architecture_32_signals_2_banks_handle_t *h,
    uint32_t gpio, /* 0..31 */
    int *value /* out */);

int tms320c6452_gpio_banked_architecture_32_signals_2_banks_enable_bank_interrupt(
    tms320c6452_gpio_banked_architecture_32_signals_2_banks_handle_t *h,
    uint32_t bank, /* 0 or 1 */
    int enable /* 0/1 */);

int tms320c6452_gpio_banked_architecture_32_signals_2_banks_configure_pin_interrupt(
    tms320c6452_gpio_banked_architecture_32_signals_2_banks_handle_t *h,
    uint32_t gpio, /* 0..31 */
    int rising /* 0/1 */, int falling /* 0/1 */);

uint32_t tms320c6452_gpio_banked_architecture_32_signals_2_banks_get_int_status(
    tms320c6452_gpio_banked_architecture_32_signals_2_banks_handle_t *h);

void tms320c6452_gpio_banked_architecture_32_signals_2_banks_clear_int_status(
    tms320c6452_gpio_banked_architecture_32_signals_2_banks_handle_t *h,
    uint32_t mask);

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_GPIO_BANKED_ARCHITECTURE_32_SIGNALS_2_BANKS_H */
