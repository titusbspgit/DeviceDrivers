/* Single-feature driver: GPIO Basic Output for TMS320C6452 DSP */
#ifndef TMS320C6452_DSP_GPIO_BASIC_OUTPUT_H
#define TMS320C6452_DSP_GPIO_BASIC_OUTPUT_H

#include <stdint.h>
#include <stdbool.h>
#include "tms320c6452_dsp_core.h"

#ifdef __cplusplus
extern "C" {
#endif

/* UNKNOWN: Actual GPIO register map offsets. Placeholders below must be replaced. */
#define GPIO_DIR_OFFSET      (0x00000000u) /* UNKNOWN */
#define GPIO_OUT_OFFSET      (0x00000000u) /* UNKNOWN */
#define GPIO_IN_OFFSET       (0x00000000u) /* UNKNOWN */
#define GPIO_OUT_SET_OFFSET  (0x00000000u) /* UNKNOWN - if write-1-set exists */
#define GPIO_OUT_CLR_OFFSET  (0x00000000u) /* UNKNOWN - if write-1-clear exists */

/* Status codes */
typedef enum {
    GPIO_BO_OK = 0,
    GPIO_BO_EINVAL = -1,
    GPIO_BO_EUNSUPPORTED = -2
} gpio_bo_status_t;

/* APIs (deterministic, no dynamic allocation) */
int tms320c6452_dsp_gpio_basic_output_init(void);
int tms320c6452_dsp_gpio_basic_output_config_pin(uint32_t pin);
int tms320c6452_dsp_gpio_basic_output_write(uint32_t pin, uint8_t level);
int tms320c6452_dsp_gpio_basic_output_toggle(uint32_t pin);
int tms320c6452_dsp_gpio_basic_output_read(uint32_t pin, uint8_t *level);
void tms320c6452_dsp_gpio_basic_output_deinit(void);

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_DSP_GPIO_BASIC_OUTPUT_H */
