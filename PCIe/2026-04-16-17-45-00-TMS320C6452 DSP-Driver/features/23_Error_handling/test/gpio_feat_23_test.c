#include "../include/gpio_feat_23.h"
#include <stdio.h>
int main(void){ gpio_feat23_log_status(TMS_GPIO_OK); gpio_feat23_log_status(TMS_GPIO_EINVAL); gpio_feat23_log_status(TMS_GPIO_ESTATE); printf("[FEAT23] PASS\n"); return 0; }
