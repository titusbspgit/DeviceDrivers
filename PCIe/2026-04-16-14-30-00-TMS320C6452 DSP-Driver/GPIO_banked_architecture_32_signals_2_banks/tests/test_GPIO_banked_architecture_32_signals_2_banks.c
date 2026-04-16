#include <stdio.h>
#include "../include/GPIO_banked_architecture_32_signals_2_banks_api.h"

int main(void)
{
    int rc = GPIO_banked_architecture_32_signals_2_banks_feature_run();
    printf("GPIO_banked_architecture_32_signals_2_banks: %s (rc=%d)\n", (rc==0)?"PASS":"FAIL", rc);
    return (rc==0)?0:1;
}
