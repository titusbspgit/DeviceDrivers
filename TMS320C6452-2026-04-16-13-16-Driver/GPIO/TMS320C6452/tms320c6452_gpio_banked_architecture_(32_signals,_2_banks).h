#ifndef TMS320C6452_GPIO_BANKED_ARCHITECTURE_32_SIGNALS_2_BANKS_H
#define TMS320C6452_GPIO_BANKED_ARCHITECTURE_32_SIGNALS_2_BANKS_H

#include <stdint.h>
#include <stdbool.h>
#include "tms320c6452_gpio_core.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Return codes */
#define TMS320C6452_GPIO_OK          (0)
#define TMS320C6452_GPIO_EINVAL     (-22)

typedef void (*tms320c6452_gpio_cb_t)(uint32_t pin, void* ctx);

/* Initialization for the banked GPIO architecture feature */
int tms320c6452_gpio_banked_arch_init(const tms320c6452_gpio_core_api_t* core);

/* Direction control (mask is 32-bit: [31:16]=Bank1, [15:0]=Bank0) */
int tms320c6452_gpio_set_outputs(uint32_t mask);
int tms320c6452_gpio_set_inputs(uint32_t mask);

/* Output data control (atomic set/clear using dedicated registers) */
int tms320c6452_gpio_set_pins(uint32_t mask);
int tms320c6452_gpio_clear_pins(uint32_t mask);

/* Read input and output status */
uint32_t tms320c6452_gpio_read_inputs(void);
uint32_t tms320c6452_gpio_read_outputs(void);

/* Edge interrupt configuration */
int tms320c6452_gpio_enable_rising(uint32_t mask);
int tms320c6452_gpio_disable_rising(uint32_t mask);
int tms320c6452_gpio_enable_falling(uint32_t mask);
int tms320c6452_gpio_disable_falling(uint32_t mask);
int tms320c6452_gpio_enable_bank_interrupts(uint32_t bank_mask); /* bit0=Bank0, bit1=Bank1 */

/* Interrupt status */
uint32_t tms320c6452_gpio_read_int_status(void);

/* ISR and callback registration */
int tms320c6452_gpio_register_callback(uint32_t pin, tms320c6452_gpio_cb_t cb, void* ctx);
void tms320c6452_gpio_isr(void);

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_GPIO_BANKED_ARCHITECTURE_32_SIGNALS_2_BANKS_H */
