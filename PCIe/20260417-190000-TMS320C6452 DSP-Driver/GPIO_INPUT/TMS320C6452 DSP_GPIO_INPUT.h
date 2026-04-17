/*
 * TMS320C6452 DSP GPIO INPUT feature - Public Header
 * Target: ARM (big-endian), Baremetal, gcc
 * Module: GPIO (C6452-style), Memory-mapped I/O
 *
 * This header provides register definitions and public APIs to configure
 * GPIO pins as inputs with optional pull configuration and read them.
 *
 * Notes:
 * - GPIO internal pull-up/down is not controlled by the GPIO module on C6452;
 *   it is configured via device pinmux/pad control. The driver exposes weak
 *   HAL hooks that platform code may override to apply pulls.
 * - Module clock enable and pinmux selection must be performed prior to use.
 *   Weak HAL hooks are provided for these as well.
 */

#ifndef TMS320C6452_DSP_GPIO_INPUT_H
#define TMS320C6452_DSP_GPIO_INPUT_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Base address for GPIO module */
#define GPIO_BASE_ADDR                  (0xA1008000UL)

/* GPIO register offsets (relative to base) */
#define GPIO_PID_OFFSET                 (0x00UL)
#define GPIO_BINTEN_OFFSET              (0x08UL)
#define GPIO_DIR01_OFFSET               (0x10UL)
#define GPIO_OUT_DATA01_OFFSET          (0x14UL)
#define GPIO_SET_DATA01_OFFSET          (0x18UL)
#define GPIO_CLR_DATA01_OFFSET          (0x1CUL)
#define GPIO_IN_DATA01_OFFSET           (0x20UL)
#define GPIO_SET_RIS_TRIG01_OFFSET      (0x24UL)
#define GPIO_CLR_RIS_TRIG01_OFFSET      (0x28UL)
#define GPIO_SET_FAL_TRIG01_OFFSET      (0x2CUL)
#define GPIO_CLR_FAL_TRIG01_OFFSET      (0x30UL)
#define GPIO_INTSTAT01_OFFSET           (0x34UL)

/* Bit helper */
#define GPIO_BIT(n)                     ((uint32_t)1UL << (uint32_t)(n))

/* Public types */

typedef enum
{
    GPIO_STATUS_OK = 0,
    GPIO_STATUS_EINVAL = 1,
    GPIO_STATUS_EHW = 2
} gpio_status_t;

typedef enum
{
    GPIO_PULL_NONE = 0,
    GPIO_PULL_UP   = 1,
    GPIO_PULL_DOWN = 2
} gpio_pull_t;

/* Public API */

/*
 * Initialize the GPIO INPUT feature.
 * Pre-conditions: platform must enable GPIO module clock and select GPIO
 * function in pinmux (hooks provided and called by configure API).
 */
void gpio_input_init(void);

/*
 * Configure a GPIO pin as input with optional pull configuration.
 * pin: 0..31 (bits map GP[0..31])
 * pull: GPIO_PULL_NONE/UP/DOWN (applied via platform hook if supported)
 */
gpio_status_t gpio_input_configure(uint32_t pin, gpio_pull_t pull);

/*
 * Read the logical level of a GPIO pin (synchronized input state).
 * level: returns 0 or 1
 */
gpio_status_t gpio_input_read(uint32_t pin, uint32_t *level);

/* Convenience accessor to retrieve current DIR01 register value. */
uint32_t gpio_get_dir_reg(void);

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_DSP_GPIO_INPUT_H */
