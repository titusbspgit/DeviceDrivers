#include <stdio.h>
#include "../include/Open-drain_interaction_readback_behavior_api.h"

int main(void)
{
    int rc = Open_drain_interaction_readback_behavior_feature_run();
    printf("Open-drain_interaction_readback_behavior: %s (rc=%d)\n", (rc==0)?"PASS":"FAIL", rc);
    return (rc==0)?0:1;
}
