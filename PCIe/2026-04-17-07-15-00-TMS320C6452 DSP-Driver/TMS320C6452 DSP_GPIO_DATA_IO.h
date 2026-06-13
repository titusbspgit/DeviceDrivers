/* RCI-ag-DrvGen Agent: GPIO Data I/O driver header for TMS320C6452 DSP */
#ifndef TMS320C6452_DSP_GPIO_DATA_IO_H
#define TMS320C6452_DSP_GPIO_DATA_IO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/* Register offsets from GPIO base (from RAG) */
#define GPIO_DIR01_OFFSET       (0x10u)
#define GPIO_OUT_DATA01_OFFSET  (0x14u)
#define GPIO_SET_DATA01_OFFSET  (0x18u)
#define GPIO_CLR_DATA01_OFFSET  (0x1Cu)
#define GPIO_IN_DATA01_OFFSET   (0x20u)

#define GPIO_PIN_MASK(pin_)     ((uint32_t)1u << (uint32_t)(pin_))
#define GPIO_MAX_PIN            (31u)

/* Status codes */
typedef enum {
    GPIO_DATA_OK = 0,
    GPIO_DATA_ERR_PARAM = -1
} gpio_data_status_t;

/* Initialize the driver with base address (0 uses default 0xA1008000) */
void gpio_data_init(uintptr_t base);

/* Write logical value (0/1) to a pin configured as output */
gpio_data_status_t gpio_write(uint32_t pin, bool value);

/* Atomic set/clear/toggle helpers */
gpio_data_status_t gpio_set(uint32_t pin);

gpio_data_status_t gpio_clear(uint32_t pin);

gpio_data_status_t gpio_toggle(uint32_t pin);

/* Read input (synchronized) and output latch values */
gpio_data_status_t gpio_read_in(uint32_t pin, bool * value);

gpio_data_status_t gpio_read_out(uint32_t pin, bool * value);

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_DSP_GPIO_DATA_IO_H */
