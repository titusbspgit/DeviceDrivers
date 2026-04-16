#include "../include/gpio_feat_05.h"
#include <stdio.h>
static int stub_psc_enable(void){ return 0; }
static int stub_psc_disable(void){ return 0; }
static int stub_pinmux_validate(void){ return 0; }
int main(void){ tms_gpio_ctx_t ctx; tms_gpio_ctx_init(&ctx,TMS_GPIO_BASE_DEFAULT,stub_psc_enable,stub_psc_disable,stub_pinmux_validate); if(tms_gpio_feature_full_init(&ctx)!=TMS_GPIO_OK){ printf("[FEAT05] init fail\n"); return -1;} if(gpio_feat05_toggle_pin(&ctx,1u)!=TMS_GPIO_OK){ printf("[FEAT05] FAIL\n"); return -2;} printf("[FEAT05] PASS\n"); return 0; }
