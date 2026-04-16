#include <stdio.h>
#include "../include/Input_sampling_IN_DATA01_api.h"

int main(void)
{
    int rc = Input_sampling_IN_DATA01_feature_run();
    printf("Input_sampling_IN_DATA01: %s (rc=%d)\n", (rc==0)?"PASS":"FAIL", rc);
    return (rc==0)?0:1;
}
