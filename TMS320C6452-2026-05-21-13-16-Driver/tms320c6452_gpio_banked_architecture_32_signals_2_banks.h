#ifndef TMS320C6452_GPIO_BANKED_ARCHITECTURE_32_SIGNALS_2_BANKS_H
#define TMS320C6452_GPIO_BANKED_ARCHITECTURE_32_SIGNALS_2_BANKS_H

#include <stdint.h>
#include <stdbool.h>
#include "tms320c6452_gpio_core.h"

/* Return codes */
#define TMS320C6452_GPIO_OK                 (0)
#define TMS320C6452_GPIO_EINVAL             (-1)

/* IRQ callback type (per-pin) */
typedef void (*tms320c6452_gpio_irq_cb_t)(uint32_t pin);

/* API: Banked architecture (32 signals, 2 banks) */
void tms320c6452_gpio_banked_init(void);

int  tms320c6452_gpio_banked_set_direction(uint32_t pin, bool input);
int  tms320c6452_gpio_banked_get_direction(uint32_t pin, bool *input);

int  tms320c6452_gpio_banked_set_pin(uint32_t pin);
int  tms320c6452_gpio_banked_clear_pin(uint32_t pin);
int  tms320c6452_gpio_banked_write_pin(uint32_t pin, uint32_t value);

int  tms320c6452_gpio_banked_read_pin(uint32_t pin, uint32_t *value);

int  tms320c6452_gpio_banked_read_bank_inputs(uint32_t bank, uint16_t *value16);
int  tms320c6452_gpio_banked_write_bank_mask(uint32_t bank,
                                             uint16_t set_mask,
                                             uint16_t clear_mask);

int  tms320c6452_gpio_banked_enable_bank_irq(uint32_t bank, bool enable);
int  tms320c6452_gpio_banked_configure_pin_irq(uint32_t pin, bool rising, bool falling);
uint32_t tms320c6452_gpio_banked_get_pending(void);
void tms320c6452_gpio_banked_clear_pending(uint32_t mask);

int  tms320c6452_gpio_banked_register_callback(uint32_t pin, tms320c6452_gpio_irq_cb_t cb);
void tms320c6452_gpio_banked_isr(void);

#endif /* TMS320C6452_GPIO_BANKED_ARCHITECTURE_32_SIGNALS_2_BANKS_H */
