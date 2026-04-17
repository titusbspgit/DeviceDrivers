/* RCI-ag-DrvGen Agent: GPIO Direction driver header for TMS320C6452 DSP */
#ifndef TMS320C6452_DSP_GPIO_DIRECTION_H
#define TMS320C6452_DSP_GPIO_DIRECTION_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/* Base address type */
typedef struct {
    volatile uint32_t DIR01;      /* Offset 0x10: Direction register; 1=input,0=output */
    volatile uint32_t OUT_DATA01; /* Offset 0x14: Output data latch */
    volatile uint32_t SET_DATA01; /* Offset 0x18: Set data */
    volatile uint32_t CLR_DATA01; /* Offset 0x1C: Clear data */
    volatile uint32_t IN_DATA01;  /* Offset 0x20: Input data */
} TMS320C6452_GPIO_Regs;

/* Register offsets (for alternative access) */
#define GPIO_DIR01_OFFSET      (0x10u)
#define GPIO_OUT_DATA01_OFFSET (0x14u)
#define GPIO_SET_DATA01_OFFSET (0x18u)
#define GPIO_CLR_DATA01_OFFSET (0x1Cu)
#define GPIO_IN_DATA01_OFFSET  (0x20u)

/* Direction bit meaning */
#define GPIO_DIR_INPUT_MASK(pin)  ((uint32_t)1u << (uint32_t)(pin))

/* Limits */
#define GPIO_MAX_PIN   (31u)

/* Error codes */
typedef enum {
    GPIO_DIR_OK = 0,
    GPIO_DIR_ERR_PARAM = -1
} gpio_dir_status_t;

/* Public API */
void gpio_dir_init(uintptr_t base);

gpio_dir_status_t gpio_set_dir(uint32_t pin, bool is_output);

gpio_dir_status_t gpio_get_dir(uint32_t pin, bool * is_output);

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_DSP_GPIO_DIRECTION_H */
