#include <stdio.h>
#include "../include/Clock_gating_and_frequency_PSC_LPSC_api.h"

int main(void)
{
    int rc = Clock_gating_and_frequency_PSC_LPSC_feature_run();
    printf("Clock_gating_and_frequency_PSC_LPSC: %s (rc=%d)\n", (rc==0)?"PASS":"FAIL", rc);
    return (rc==0)?0:1;
}
