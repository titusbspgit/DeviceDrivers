#ifndef GPIO_FEAT_23_H
#define GPIO_FEAT_23_H
#include "../../../common/include/tms320c6452_gpio.h"
#include <stdio.h>
static inline void gpio_feat23_log_status(tms_gpio_status_t st){ printf("[FEAT23] status=%u\n", (unsigned)st); }
#endif
