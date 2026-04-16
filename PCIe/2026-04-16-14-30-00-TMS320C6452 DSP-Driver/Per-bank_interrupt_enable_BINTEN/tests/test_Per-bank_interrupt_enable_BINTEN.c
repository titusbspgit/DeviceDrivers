#include <stdio.h>
#include "../include/Per-bank_interrupt_enable_BINTEN_api.h"

int main(void)
{
    int rc = Per_bank_interrupt_enable_BINTEN_feature_run();
    printf("Per-bank_interrupt_enable_BINTEN: %s (rc=%d)\n", (rc==0)?"PASS":"FAIL", rc);
    return (rc==0)?0:1;
}
