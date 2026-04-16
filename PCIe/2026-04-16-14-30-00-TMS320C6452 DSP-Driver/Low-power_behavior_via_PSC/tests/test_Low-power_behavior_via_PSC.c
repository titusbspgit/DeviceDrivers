#include <stdio.h>
#include "../include/Low-power_behavior_via_PSC_api.h"

int main(void)
{
    int rc = Low_power_behavior_via_PSC_feature_run();
    printf("Low-power_behavior_via_PSC: %s (rc=%d)\n", (rc==0)?"PASS":"FAIL", rc);
    return (rc==0)?0:1;
}
