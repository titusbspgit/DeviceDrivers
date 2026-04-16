#include "../include/gpio_feat_03.h"
#include <stdio.h>
extern void gpio_feat03_log(void);
int main(void){ if (gpio_feat03_check()!=TMS_GPIO_OK){ printf("[FEAT03] FAIL\n"); return -1;} gpio_feat03_log(); printf("[FEAT03] PASS\n"); return 0; }
