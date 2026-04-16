#include <stdio.h>
#include "../include/Direction_control_DIR01_api.h"

int main(void)
{
    int rc = Direction_control_DIR01_feature_run();
    printf("Direction_control_DIR01: %s (rc=%d)\n", (rc==0)?"PASS":"FAIL", rc);
    return (rc==0)?0:1;
}
