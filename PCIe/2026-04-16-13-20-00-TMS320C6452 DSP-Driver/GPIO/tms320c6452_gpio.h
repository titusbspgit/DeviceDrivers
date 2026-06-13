/*
 * TMS320C6452 GPIO Driver - Header
 * RCI-ag-DrvGen Agent
 * Target: ARM, Baremetal, gcc, Big Endian (operation is endianness-independent per SPRUF95)
 * No dynamic allocation; memory-mapped I/O via base address
 */
#ifndef TMS320C6452_GPIO_H
#define TMS320C6452_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Default base (from provided environment) */
#define TMS320C6452_GPIO_BASE_DEFAULT   (0xA1008000u)

/* Register offsets (32-bit registers) per SPRUF95 */
#define TMS320C6452_GPIO_PID_OFFSET             (0x00u)
#define TMS320C6452_GPIO_BINTEN_OFFSET          (0x08u)
#define TMS320C6452_GPIO_DIR01_OFFSET           (0x10u)
#define TMS320C6452_GPIO_OUT_DATA01_OFFSET      (0x14u)
#define TMS320C6452_GPIO_SET_DATA01_OFFSET      (0x18u)
#define TMS320C6452_GPIO_CLR_DATA01_OFFSET      (0x1Cu)
#define TMS320C6452_GPIO_IN_DATA01_OFFSET       (0x20u)
#define TMS320C6452_GPIO_SET_RIS_TRIG01_OFFSET  (0x24u)
#define TMS320C6452_GPIO_CLR_RIS_TRIG01_OFFSET  (0x28u)
#define TMS320C6452_GPIO_SET_FAL_TRIG01_OFFSET  (0x2Cu)
#define TMS320C6452_GPIO_CLR_FAL_TRIG01_OFFSET  (0x30u)
#define TMS320C6452_GPIO_INTSTAT01_OFFSET       (0x34u)

/* Bit helpers */
#define TMS320C6452_GPIO_PIN_MASK(pin)   ( (uint32_t)1u << (uint32_t)(pin) )

/* BINTEN enables */
#define TMS320C6452_GPIO_BINTEN_EN0   (0x00000001u)
#define TMS320C6452_GPIO_BINTEN_EN1   (0x00000002u)

/* PID field masks (for identification) */
#define TMS320C6452_GPIO_PID_SCHEME_MASK    (0xC0000000u)
#define TMS320C6452_GPIO_PID_SCHEME_SHIFT   (30u)
#define TMS320C6452_GPIO_PID_FUNCTION_MASK  (0x0FFF0000u)
#define TMS320C6452_GPIO_PID_FUNCTION_SHIFT (16u)
#define TMS320C6452_GPIO_PID_MAJOR_MASK     (0x00000700u)
#define TMS320C6452_GPIO_PID_MAJOR_SHIFT    (8u)
#define TMS320C6452_GPIO_PID_MINOR_MASK     (0x0000003Fu)
#define TMS320C6452_GPIO_PID_MINOR_SHIFT    (0u)

#define TMS320C6452_GPIO_PID_FUNCTION_GPIO  (0x0483u)

/* Banks */
typedef enum
{
    TMS320C6452_GPIO_BANK0 = 0,
    TMS320C6452_GPIO_BANK1 = 1
} tms320c6452_gpio_bank_t;

/* Status codes */
typedef enum
{
    TMS320C6452_GPIO_STATUS_OK = 0,
    TMS320C6452_GPIO_STATUS_EINVAL = 1,   /* invalid argument */
    TMS320C6452_GPIO_STATUS_EHWID = 2     /* unexpected peripheral ID */
} tms320c6452_gpio_status_t;

/* Driver context: holds base address only; no dynamic allocation */
typedef struct
{
    uintptr_t base; /* base physical address of GPIO module */
} tms320c6452_gpio_t;

/* HAL register accessors (opaque to user, provided for test introspection) */
static inline void tms320c6452_gpio_reg_write(volatile tms320c6452_gpio_t * const ctx, uint32_t reg_off, uint32_t val)
{
    volatile uint32_t * const reg = (volatile uint32_t *)(ctx->base + (uintptr_t)reg_off);
    *reg = val;
}

static inline uint32_t tms320c6452_gpio_reg_read(volatile const tms320c6452_gpio_t * const ctx, uint32_t reg_off)
{
    volatile const uint32_t * const reg = (volatile const uint32_t *)(ctx->base + (uintptr_t)reg_off);
    return *reg;
}

/* Public API */

/* Initialize driver context with base; verify PID function if verify_pid==true */
tms320c6452_gpio_status_t tms320c6452_gpio_init(tms320c6452_gpio_t * ctx, uintptr_t base, bool verify_pid);

/* Read back PID register */
uint32_t tms320c6452_gpio_get_pid(const tms320c6452_gpio_t * ctx);

/* Direction control */
/* Configure one pin as input (true) or output (false) */
tms320c6452_gpio_status_t tms320c6452_gpio_set_pin_direction(const tms320c6452_gpio_t * ctx, uint32_t pin, bool input);
/* Batch: set mask bits to inputs (1) */
tms320c6452_gpio_status_t tms320c6452_gpio_set_inputs_mask(const tms320c6452_gpio_t * ctx, uint32_t mask);
/* Batch: set mask bits to outputs (0) */
tms320c6452_gpio_status_t tms320c6452_gpio_set_outputs_mask(const tms320c6452_gpio_t * ctx, uint32_t mask);

/* Output control (atomic via SET/CLR) */
tms320c6452_gpio_status_t tms320c6452_gpio_write_pin(const tms320c6452_gpio_t * ctx, uint32_t pin, bool high);
tms320c6452_gpio_status_t tms320c6452_gpio_set_pins(const tms320c6452_gpio_t * ctx, uint32_t mask);
tms320c6452_gpio_status_t tms320c6452_gpio_clear_pins(const tms320c6452_gpio_t * ctx, uint32_t mask);
tms320c6452_gpio_status_t tms320c6452_gpio_toggle_pin(const tms320c6452_gpio_t * ctx, uint32_t pin);

/* Input/readback */
tms320c6452_gpio_status_t tms320c6452_gpio_read_pin(const tms320c6452_gpio_t * ctx, uint32_t pin, bool * level);
uint32_t tms320c6452_gpio_read_inputs(const tms320c6452_gpio_t * ctx);
uint32_t tms320c6452_gpio_read_outputs_latch(const tms320c6452_gpio_t * ctx);

/* Interrupt configuration and status */
tms320c6452_gpio_status_t tms320c6452_gpio_bank_enable(const tms320c6452_gpio_t * ctx, tms320c6452_gpio_bank_t bank, bool enable);
tms320c6452_gpio_status_t tms320c6452_gpio_config_edge(const tms320c6452_gpio_t * ctx, uint32_t pin, bool enable_rising, bool enable_falling);
uint32_t tms320c6452_gpio_get_pending(const tms320c6452_gpio_t * ctx);
void tms320c6452_gpio_clear_pending(const tms320c6452_gpio_t * ctx, uint32_t mask);

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_GPIO_H */
