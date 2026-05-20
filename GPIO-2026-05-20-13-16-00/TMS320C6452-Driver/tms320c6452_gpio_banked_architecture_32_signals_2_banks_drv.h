#ifndef TMS320C6452_GPIO_BANKED_ARCHITECTURE_32_SIGNALS_2_BANKS_DRV_H
#define TMS320C6452_GPIO_BANKED_ARCHITECTURE_32_SIGNALS_2_BANKS_DRV_H

#include <stdint.h>
#include <stdbool.h>
#include "tms320c6452_gpio_core.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TMS320C6452_GPIO_BANK0  (0u)
#define TMS320C6452_GPIO_BANK1  (1u)

#define TMS320C6452_GPIO_OK          (0)
#define TMS320C6452_GPIO_ERR_INVAL   (-1)

typedef void (*tms320c6452_gpio_bank_isr_cb_t)(uint16_t status, void *ctx);

int tms320c6452_gpio_banked_arch_init(void);

int      tms320c6452_gpio_banked_arch_set_bank_dir(uint8_t bank, uint16_t dir_mask);
uint16_t tms320c6452_gpio_banked_arch_bank_read_dir(uint8_t bank);

int tms320c6452_gpio_banked_arch_bank_set_bits(uint8_t bank, uint16_t set_mask);
int tms320c6452_gpio_banked_arch_bank_clear_bits(uint8_t bank, uint16_t clr_mask);
int tms320c6452_gpio_banked_arch_bank_write_out_mask(uint8_t bank, uint16_t value, uint16_t mask);

uint16_t tms320c6452_gpio_banked_arch_bank_read_input(uint8_t bank);
uint16_t tms320c6452_gpio_banked_arch_bank_read_output(uint8_t bank);

int tms320c6452_gpio_banked_arch_bank_enable_rising(uint8_t bank, uint16_t mask);
int tms320c6452_gpio_banked_arch_bank_disable_rising(uint8_t bank, uint16_t mask);
int tms320c6452_gpio_banked_arch_bank_enable_falling(uint8_t bank, uint16_t mask);
int tms320c6452_gpio_banked_arch_bank_disable_falling(uint8_t bank, uint16_t mask);

int      tms320c6452_gpio_banked_arch_enable_bank_interrupt(uint8_t bank);
int      tms320c6452_gpio_banked_arch_disable_bank_interrupt(uint8_t bank);
uint32_t tms320c6452_gpio_banked_arch_get_int_status(void);

int  tms320c6452_gpio_banked_arch_register_isr(uint8_t bank, tms320c6452_gpio_bank_isr_cb_t cb, void *ctx);
void tms320c6452_gpio_banked_arch_bank_isr(uint8_t bank);
void tms320c6452_gpio_banked_arch_bank0_isr(void);
void tms320c6452_gpio_banked_arch_bank1_isr(void);

int tms320c6452_gpio_banked_arch_edge_ack(uint8_t bank, uint16_t mask);

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_GPIO_BANKED_ARCHITECTURE_32_SIGNALS_2_BANKS_DRV_H */
