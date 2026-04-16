#include "../include/gpio_feat_04.h"
#include <stdio.h>
static int stub_psc_enable(void){ return 0; }
static int stub_psc_disable(void){ return 0; }
static int stub_pinmux_validate(void){ return 0; }
int main(void){ tms_gpio_ctx_t ctx; tms_gpio_ctx_init(&ctx,TMS_GPIO_BASE_DEFAULT,stub_psc_enable,stub_psc_disable,stub_pinmux_validate); if(tms_gpio_feature_full_init(&ctx)!=TMS_GPIO_OK){printf("[FEAT04] init fail\n"); return -1;} if(gpio_feat04_set_get_dir(&ctx,0u,false)!=TMS_GPIO_OK){printf("[FEAT04] out cfg fail\n"); return -2;} if(gpio_feat04_set_get_dir(&ctx,0u,true)!=TMS_GPIO_OK){printf("[FEAT04] in cfg fail\n"); return -3;} printf("[FEAT04] PASS\n"); return 0; }
