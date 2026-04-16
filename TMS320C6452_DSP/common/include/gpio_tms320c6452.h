/*
 * TMS320C6452 GPIO Driver - Header (Common)
 * Environment: Bare-metal, big-endian, GCC
 * Controller base: 0xA1008000
 *
 * Author: RCI-ag-DrvGen Agent
 */
#ifndef TMS320C6452_GPIO_H
#define TMS320C6452_GPIO_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Base address (can be overridden at init) */
#define TMS320C6452_GPIO_BASE_ADDR   ( (uintptr_t)0xA1008000u )

/* Register offsets (from base) */
#define GPIO_REG_PID_OFFSET              (0x00u)
#define GPIO_REG_BINTEN_OFFSET           (0x08u)
#define GPIO_REG_DIR01_OFFSET            (0x10u)
#define GPIO_REG_OUT_DATA01_OFFSET       (0x14u)
#define GPIO_REG_SET_DATA01_OFFSET       (0x18u)
#define GPIO_REG_CLR_DATA01_OFFSET       (0x1Cu)
#define GPIO_REG_IN_DATA01_OFFSET        (0x20u)
#define GPIO_REG_SET_RIS_TRIG01_OFFSET   (0x24u)
#define GPIO_REG_CLR_RIS_TRIG01_OFFSET   (0x28u)
#define GPIO_REG_SET_FAL_TRIG01_OFFSET   (0x2Cu)
#define GPIO_REG_CLR_FAL_TRIG01_OFFSET   (0x30u)
#define GPIO_REG_INTSTAT01_OFFSET        (0x34u)

/* BINTEN bits */
#define GPIO_BINTEN_EN0                  (0x00000001u)
#define GPIO_BINTEN_EN1                  (0x00000002u)

/* Masks and helpers */
#define GPIO_PIN_MAX                     (32u)
#define GPIO_BANKS                       (2u)
#define GPIO_PINS_PER_BANK               (16u)
#define GPIO_PIN_MASK(pin)               ((uint32_t)1u << ((uint32_t)(pin) & 31u))
#define GPIO_BANK_FROM_PIN(pin)          ((uint32_t)(pin) / GPIO_PINS_PER_BANK)

/* Expected PID reset value (from spec) */
#define GPIO_PID_RESET_VALUE             (0x44830105u)

/* Status codes */
typedef enum
{
    GPIO_STATUS_OK = 0,
    GPIO_STATUS_INVALID_ARG = 1,
    GPIO_STATUS_NOT_INITIALIZED = 2,
    GPIO_STATUS_HW_MISMATCH = 3
} gpio_status_t;

/* Direction */
typedef enum
{
    GPIO_DIR_OUTPUT = 0,
    GPIO_DIR_INPUT  = 1
} gpio_dir_t;

/* Edge selection for interrupts */
typedef enum
{
    GPIO_EDGE_NONE    = 0,
    GPIO_EDGE_RISING  = 1,
    GPIO_EDGE_FALLING = 2,
    GPIO_EDGE_BOTH    = 3
} gpio_edge_t;

/* API: Initialization and identification */
gpio_status_t gpio_init(uintptr_t base);
gpio_status_t gpio_get_pid(uint32_t *pid_out);

/* API: Direction and I/O */
gpio_status_t gpio_set_direction(uint32_t pin, gpio_dir_t dir);
gpio_status_t gpio_get_direction(uint32_t pin, gpio_dir_t *dir_out);
gpio_status_t gpio_write_pin(uint32_t pin, bool value);
gpio_status_t gpio_read_pin(uint32_t pin, bool *value_out);

/* API: Bulk output using atomic set/clear */
gpio_status_t gpio_write_mask(uint32_t set_mask, uint32_t clear_mask);

/* API: Interrupt configuration and status */
gpio_status_t gpio_irq_configure_pin(uint32_t pin, gpio_edge_t edge_sel);
gpio_status_t gpio_irq_bank_enable(uint32_t bank, bool enable);
gpio_status_t gpio_irq_get_status(uint32_t *pending_mask_out);
gpio_status_t gpio_irq_clear_status(uint32_t clear_mask);

/* ISR helper for a given bank: returns cleared mask for that bank */
gpio_status_t gpio_isr_service(uint32_t bank, uint32_t *cleared_mask_out);

/* Platform HAL hooks (weak, may be overridden) */
/* Enable GPIO module clock via PSC/LPSC; return true on success */
bool gpio_hal_enable_clock(void);
/* Configure pin multiplexing for a GPIO pin; return true on success */
bool gpio_hal_configure_pinmux(uint32_t pin);

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_GPIO_H */
