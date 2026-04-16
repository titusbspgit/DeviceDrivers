#ifndef GPIO_FEAT_22_H
#define GPIO_FEAT_22_H
#include <stdint.h>
#include <stdio.h>
#include "../../../common/include/tms320c6452_gpio.h"
static inline void gpio_feat22_print_base(uintptr_t base){ printf("[FEAT22] Base=0x%lX (default=0x%lX)\n", (unsigned long)base, (unsigned long)TMS_GPIO_BASE_DEFAULT); }
#endif
