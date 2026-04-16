#include <stdio.h>
#include <stdint.h>
#include "../include/feature_regmap.h"

int main(void)
{
    printf("Offsets: PID=0x%02X DIR=0x%02X INTSTAT=0x%02X\n", GPIO_REG_PID_OFFSET, GPIO_REG_DIR01_OFFSET, GPIO_REG_INTSTAT01_OFFSET);
    printf("PASS\n");
    return 0;
}
