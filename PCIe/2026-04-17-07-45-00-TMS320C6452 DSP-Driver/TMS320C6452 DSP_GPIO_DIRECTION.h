/* RCI-ag-DrvGen Agent: GPIO Direction driver header for TMS320C6452 DSP */
#ifndef TMS320C6452_DSP_GPIO_DIRECTION_H
#define TMS320C6452_DSP_GPIO_DIRECTION_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/* Register offsets from GPIO base (verified via RAG) */
#define GPIO_DIR01_OFFSET       (0x10u) /* Direction: 1=input, 0=output */
#define GPIO_OUT_DATA01_OFFSET  (0x14u) /* Output data latch */
#define GPIO_SET_DATA01_OFFSET  (0x18u) /* Set data (data only) */
#define GPIO_CLR_DATA01_OFFSET  (0x1Cu) /* Clear data (data only) */
#define GPIO_IN_DATA01_OFFSET   (0x20u) /* Synchronized input */

/* Direction bit helper */
#define GPIO_PIN_MASK(pin_)     ((uint32_t)1u << (uint32_t)(pin_))

/* Limits */
#define GPIO_MAX_PIN            (31u)

/* Status codes */
typedef enum {
    GPIO_DIR_OK = 0,
    GPIO_DIR_ERR_PARAM = -1
} gpio_dir_status_t;

/* Public API */
/*
 * Initialize the GPIO direction driver with a base address. If base == 0, a
 * platform default base is used (0xA1008000 as per provided configuration).
 */
void gpio_dir_init(uintptr_t base);

/* Configure a pin's direction (true: output, false: input). */
gpio_dir_status_t gpio_set_dir(uint32_t pin, bool is_output);

/* Read back a pin's direction into *is_output (true: output). */
gpio_dir_status_t gpio_get_dir(uint32_t pin, bool * is_output);

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_DSP_GPIO_DIRECTION_H */
