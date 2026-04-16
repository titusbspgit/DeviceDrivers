#include "../include/gpio_feat_06.h"
#include <stdio.h>
static int stub_psc_enable(void){ return 0; }
static int stub_psc_disable(void){ return 0; }
static int stub_pinmux_validate(void){ return 0; }
int main(void){ tms_gpio_ctx_t ctx; bool lvl=false; tms_gpio_ctx_init(&ctx,TMS_GPIO_BASE_DEFAULT,stub_psc_enable,stub_psc_disable,stub_pinmux_validate); if(tms_gpio_feature_full_init(&ctx)!=TMS_GPIO_OK){ printf("[FEAT06] init fail\n"); return -1;} if(gpio_feat06_sample_pin(&ctx,2u,&lvl)!=TMS_GPIO_OK){ printf("[FEAT06] FAIL\n"); return -2;} printf("[FEAT06] PASS\n"); return 0; }
