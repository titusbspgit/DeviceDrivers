#include "../include/gpio_feat_10.h"
#include <stdio.h>
static int stub_psc_enable(void){ return 0; }
static int stub_psc_disable(void){ return 0; }
static int stub_pinmux_validate(void){ return 0; }
int main(void){ tms_gpio_ctx_t ctx; tms_gpio_ctx_init(&ctx,TMS_GPIO_BASE_DEFAULT,stub_psc_enable,stub_psc_disable,stub_pinmux_validate); if(tms_gpio_feature_full_init(&ctx)!=TMS_GPIO_OK){ printf("[FEAT10] init fail\n"); return -1;} if(gpio_feat10_prepare_edma_use(&ctx,0u)!=TMS_GPIO_OK){ printf("[FEAT10] FAIL\n"); return -2;} printf("[FEAT10] PASS (route EDMA externally)\n"); return 0; }
