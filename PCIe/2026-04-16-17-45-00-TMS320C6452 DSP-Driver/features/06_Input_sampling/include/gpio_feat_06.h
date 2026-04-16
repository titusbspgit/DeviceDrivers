#ifndef GPIO_FEAT_06_H
#define GPIO_FEAT_06_H
#include "../../../common/include/tms320c6452_gpio.h"
#include <stdbool.h>

tms_gpio_status_t gpio_feat06_sample_pin(tms_gpio_ctx_t *ctx, uint32_t pin, bool *level);

#endif
