#include <stdio.h>
#include "../include/Output_drive_control_OUT_SET_CLR_api.h"

int main(void)
{
    int rc = Output_drive_control_OUT_SET_CLR_feature_run();
    printf("Output_drive_control_OUT_SET_CLR: %s (rc=%d)\n", (rc==0)?"PASS":"FAIL", rc);
    return (rc==0)?0:1;
}
