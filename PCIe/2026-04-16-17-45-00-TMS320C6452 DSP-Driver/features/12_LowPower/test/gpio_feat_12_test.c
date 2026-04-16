#include "../include/gpio_feat_12.h"
#include <stdio.h>
static int stub_psc_enable(void){ return 0; }
static int stub_psc_disable(void){ return 0; }
static int stub_pinmux_validate(void){ return 0; }
int main(void){ tms_gpio_ctx_t ctx; tms_gpio_ctx_init(&ctx,TMS_GPIO_BASE_DEFAULT,stub_psc_enable,stub_psc_disable,stub_pinmux_validate); if(tms_gpio_feature_full_init(&ctx)!=TMS_GPIO_OK){ printf("[FEAT12] init fail\n"); return -1;} if(gpio_feat12_low_power_cycle(&ctx)!=TMS_GPIO_OK){ printf("[FEAT12] FAIL\n"); return -2;} printf("[FEAT12] PASS\n"); return 0; }
