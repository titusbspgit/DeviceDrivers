#include <stdio.h>
#include "../include/Interrupt_status_and_W1C_INTSTAT01_api.h"

int main(void)
{
    int rc = Interrupt_status_and_W1C_INTSTAT01_feature_run();
    printf("Interrupt_status_and_W1C_INTSTAT01: %s (rc=%d)\n", (rc==0)?"PASS":"FAIL", rc);
    return (rc==0)?0:1;
}
