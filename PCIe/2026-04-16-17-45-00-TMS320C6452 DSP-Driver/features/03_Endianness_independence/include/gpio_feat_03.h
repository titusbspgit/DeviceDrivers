#ifndef GPIO_FEAT_03_H
#define GPIO_FEAT_03_H
#include "../../../common/include/tms320c6452_gpio.h"
static inline tms_gpio_status_t gpio_feat03_check(void){ return tms_gpio_feature_endianness_check(); }
#endif
