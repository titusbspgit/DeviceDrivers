#ifndef TMS320C6452_DSP_GPIO_BASIC_H
#define TMS320C6452_DSP_GPIO_BASIC_H

// GPIO Basic feature header (single feature)
// Registers/bitfields are UNKNOWN where silicon RAG data is unavailable.

#include <stdint.h>
#include <stdbool.h>
#include "TMS320C6452_DSP_core.h"

#ifdef __cplusplus
extern "C" {
#endif

// Offsets (UNKNOWN placeholders)
#define GPIO_DIR_OFFSET      (TMS320C6452_DSP_REG_UNKNOWN) /* UNKNOWN */
#define GPIO_OUT_OFFSET      (TMS320C6452_DSP_REG_UNKNOWN) /* UNKNOWN */
#define GPIO_IN_OFFSET       (TMS320C6452_DSP_REG_UNKNOWN) /* UNKNOWN */
#define GPIO_INT_EN_OFFSET   (TMS320C6452_DSP_REG_UNKNOWN) /* UNKNOWN */
#define GPIO_INT_STS_OFFSET  (TMS320C6452_DSP_REG_UNKNOWN) /* UNKNOWN */
#define GPIO_INT_CLR_OFFSET  (TMS320C6452_DSP_REG_UNKNOWN) /* UNKNOWN */

// Bitfield helpers (generic)
#define GPIO_PIN_MASK(n)     ( (uint32_t)(1u << ((uint32_t)(n) & 31u)) )

// Driver return codes
typedef enum {
    GPIO_BASIC_OK = 0,
    GPIO_BASIC_ERR_UNKNOWN_REG = -1,
    GPIO_BASIC_ERR_BAD_PIN     = -2,
} gpio_basic_status_t;

// Direction
typedef enum {
    GPIO_DIR_INPUT  = 0,
    GPIO_DIR_OUTPUT = 1,
} gpio_dir_t;

// API
int  TMS320C6452_DSP_gpio_basic_init(const TMS320C6452_DSP_CoreAPI *core);
int  TMS320C6452_DSP_gpio_basic_configure(const TMS320C6452_DSP_CoreAPI *core, uint32_t pin, gpio_dir_t dir);
int  TMS320C6452_DSP_gpio_basic_write(const TMS320C6452_DSP_CoreAPI *core, uint32_t pin, bool high);
int  TMS320C6452_DSP_gpio_basic_read(const TMS320C6452_DSP_CoreAPI *core, uint32_t pin, bool *value);
int  TMS320C6452_DSP_gpio_basic_int_enable(const TMS320C6452_DSP_CoreAPI *core, uint32_t pin, bool enable);
int  TMS320C6452_DSP_gpio_basic_int_clear(const TMS320C6452_DSP_CoreAPI *core, uint32_t pin);

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_DSP_GPIO_BASIC_H */
