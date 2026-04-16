#include <stdio.h>
#include "../include/Emulation_suspend_behavior_api.h"

int main(void)
{
    int rc = Emulation_suspend_behavior_feature_run();
    printf("Emulation_suspend_behavior: %s (rc=%d)\n", (rc==0)?"PASS":"FAIL", rc);
    return (rc==0)?0:1;
}
