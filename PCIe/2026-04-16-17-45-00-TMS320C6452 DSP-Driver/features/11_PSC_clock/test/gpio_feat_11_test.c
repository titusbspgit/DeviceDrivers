#include "../include/gpio_feat_11.h"
#include <stdio.h>
static int stub_psc_enable(void){ return 0; }
static int stub_psc_disable(void){ return 0; }
static int stub_pinmux_validate(void){ return 0; }
int main(void){ tms_gpio_ctx_t ctx; tms_gpio_ctx_init(&ctx,TMS_GPIO_BASE_DEFAULT,stub_psc_enable,stub_psc_disable,stub_pinmux_validate); if(gpio_feat11_clock_enable_disable(&ctx)!=TMS_GPIO_OK){ printf("[FEAT11] FAIL\n"); return -1;} printf("[FEAT11] PASS\n"); return 0; }
