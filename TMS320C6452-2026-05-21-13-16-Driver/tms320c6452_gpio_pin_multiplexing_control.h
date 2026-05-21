#ifndef TMS320C6452_GPIO_PIN_MULTIPLEXING_CONTROL_H
#define TMS320C6452_GPIO_PIN_MULTIPLEXING_CONTROL_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Controller: TMS320C6452 GPIO */
/* Feature: Pin multiplexing control */

/* Error codes */
#define TMS320C6452_GPIO_PINMUX_OK              (0)
#define TMS320C6452_GPIO_PINMUX_EINVAL         (-1)  /* Invalid argument */
#define TMS320C6452_GPIO_PINMUX_ESTATE         (-2)  /* Invalid state / not initialized or mapping incomplete */
#define TMS320C6452_GPIO_PINMUX_EUNSUPPORTED   (-3)  /* Unsupported or unknown configuration */

/* Placeholder for unknown base/register (must be provided by integrator) */
#define TMS320C6452_GPIO_PINMUX_REG_UNKNOWN ((volatile uint32_t*)0)

/* Forward declaration of field descriptor */
typedef struct tms320c6452_gpio_pinmux_field {
    volatile uint32_t* reg; /* Address of the memory-mapped pinmux register controlling this GPIO's mux field. UNKNOWN if NULL. */
    uint8_t lsb;            /* Least-significant bit position of the field */
    uint8_t width;          /* Field width in bits (e.g., 2). 0 indicates UNKNOWN */
} tms320c6452_gpio_pinmux_field_t;

/* Configuration for the pinmux feature */
typedef struct tms320c6452_gpio_pinmux_cfg {
    const tms320c6452_gpio_pinmux_field_t* map; /* Array of per-GPIO field descriptors */
    uint32_t count;                             /* Number of GPIO entries in map */
} tms320C6452_gpio_pinmux_cfg_t;

/* API */
int tms320c6452_gpio_pinmux_init(const tms320C6452_gpio_pinmux_cfg_t* cfg);
int tms320c6452_gpio_pinmux_set(uint32_t gpio_index, uint32_t function_code);
int tms320c6452_gpio_pinmux_get(uint32_t gpio_index, uint32_t* out_function_code);
bool tms320c6452_gpio_pinmux_is_initialized(void);

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_GPIO_PIN_MULTIPLEXING_CONTROL_H */
