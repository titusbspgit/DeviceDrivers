#ifndef GPIO_FEAT_17_H
#define GPIO_FEAT_17_H
#include "../../../common/include/tms320c6452_gpio.h"

tms_gpio_status_t gpio_feat17_atomic_setclr(tms_gpio_ctx_t *ctx, uint32_t set_mask, uint32_t clr_mask);

#endif
