#include <stdio.h>
#include "../include/Reset_behavior_software_vs_hardware_api.h"

int main(void)
{
    int rc = Reset_behavior_software_vs_hardware_feature_run();
    printf("Reset_behavior_software_vs_hardware: %s (rc=%d)\n", (rc==0)?"PASS":"FAIL", rc);
    return (rc==0)?0:1;
}
