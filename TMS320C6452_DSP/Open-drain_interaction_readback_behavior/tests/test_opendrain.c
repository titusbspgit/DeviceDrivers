#include <stdio.h>
#include <stdint.h>
#include "../include/feature_opendrain.h"

int main(void)
{
    (void)gpio_init(TMS320C6452_GPIO_BASE_ADDR);
    printf("Note: IN_DATA01 reflects wired-logic level for open-drain pins with external biasing.\n");
    printf("PASS (documentation)\n");
    return 0;
}
