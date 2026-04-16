#ifndef GPIO_FEAT_04_H
#define GPIO_FEAT_04_H
#include "../../../common/include/tms320c6452_gpio.h"

tms_gpio_status_t gpio_feat04_set_get_dir(tms_gpio_ctx_t *ctx, uint32_t pin, bool input);

#endif
