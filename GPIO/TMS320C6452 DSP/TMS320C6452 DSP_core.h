/*
 * TMS320C6452 DSP GPIO Core Driver - Header
 * Environment: ARM (baremetal), GCC, Big-endian
 * Note: Register offsets are placeholders; verify against device TRM. APIs are stable.
 */
#ifndef TMS320C6452_DSP_CORE_H
#define TMS320C6452_DSP_CORE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Base address (configured) */
#define TMS320C6452_DSP_GPIO_BASE      (0xA1008000u)

/* GPIO topology */
#define GPIO_MAX_PINS                  (32u)
#define GPIO_PINS_PER_BANK             (16u)
#define GPIO_NUM_BANKS                 (2u)

/* Register offsets (TODO: verify against TRM; reset values UNKNOWN) */
#define GPIO_BINTEN_OFFSET             (0x0008u)  /* Banked interrupt enable (per-bank) */
#define GPIO_DIR01_OFFSET              (0x0010u)  /* Direction: 1=input, 0=output for GP[31:0] */
#define GPIO_OUT_DATA01_OFFSET         (0x0014u)  /* Output latch state for GP[31:0] */
#define GPIO_SET_DATA01_OFFSET         (0x0018u)  /* Write 1 to set output bit */
#define GPIO_CLR_DATA01_OFFSET         (0x001Cu)  /* Write 1 to clear output bit */
#define GPIO_IN_DATA01_OFFSET          (0x0020u)  /* Input sample state for GP[31:0] */

/* Derived addresses */
#define GPIO_REG_ADDR(offset_)         ((uintptr_t)(TMS320C6452_DSP_GPIO_BASE + (uint32_t)(offset_)))

/* BINTEN per-bank bit definitions */
#define GPIO_BINTEN_BANK0              (1u << 0)
#define GPIO_BINTEN_BANK1              (1u << 1)

/* Core ops interface */
typedef struct gpio_core_ops
{
    /* Raw register accessors (offset is from GPIO base) */
    void     (*reg_write8)(uintptr_t offset, uint8_t value);
    uint8_t  (*reg_read8)(uintptr_t offset);

    void     (*reg_write16)(uintptr_t offset, uint16_t value);
    uint16_t (*reg_read16)(uintptr_t offset);

    void     (*reg_write32)(uintptr_t offset, uint32_t value);
    uint32_t (*reg_read32)(uintptr_t offset);

    /* Per-pin operations (pin: 0..31) */
    void     (*pin_set)(uint32_t pin);
    void     (*pin_clear)(uint32_t pin);
    bool     (*pin_read)(uint32_t pin);

    /* Direction control: input=true, output=false */
    void     (*pin_direction_set)(uint32_t pin, bool input);
    bool     (*pin_direction_get)(uint32_t pin);

    /* Per-bank helpers (bank: 0..1) - 16-bit significant in LSBs */
    uint32_t (*bank_read_inputs)(uint32_t bank);
    uint32_t (*bank_read_outputs)(uint32_t bank);
    void     (*bank_set_mask)(uint32_t bank, uint16_t mask);
    void     (*bank_clear_mask)(uint32_t bank, uint16_t mask);
} gpio_core_ops_t;

/* Public API: concrete ops instance */
extern const gpio_core_ops_t gpio_core;

/* Optional compiler I/O barrier for ordering */
void gpio_io_barrier(void);

/* Thin wrapper APIs on top of ops */
void     gpio_reg_write8(uintptr_t offset, uint8_t value);
uint8_t  gpio_reg_read8(uintptr_t offset);

void     gpio_reg_write16(uintptr_t offset, uint16_t value);
uint16_t gpio_reg_read16(uintptr_t offset);

void     gpio_reg_write32(uintptr_t offset, uint32_t value);
uint32_t gpio_reg_read32(uintptr_t offset);

void gpio_set_pin(uint32_t pin);
void gpio_clear_pin(uint32_t pin);
bool gpio_read_pin(uint32_t pin);

void gpio_set_direction(uint32_t pin, bool input);
bool gpio_get_direction(uint32_t pin);

uint32_t gpio_read_bank_inputs(uint32_t bank);
uint32_t gpio_read_bank_outputs(uint32_t bank);
void     gpio_bank_set_mask(uint32_t bank, uint16_t mask);
void     gpio_bank_clear_mask(uint32_t bank, uint16_t mask);

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_DSP_CORE_H */
