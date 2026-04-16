#include "../include/gpio_feat_14.h"
#include <stdio.h>
static int stub_psc_enable(void){ return 0; }
static int stub_psc_disable(void){ return 0; }
static int stub_pinmux_validate(void){ return 0; }
int main(void){ tms_gpio_ctx_t ctx; tms_gpio_ctx_init(&ctx,TMS_GPIO_BASE_DEFAULT,stub_psc_enable,stub_psc_disable,stub_pinmux_validate); if(gpio_feat14_full_init(&ctx)!=TMS_GPIO_OK){ printf("[FEAT14] FAIL\n"); return -1;} printf("[FEAT14] PASS\n"); return 0; }
