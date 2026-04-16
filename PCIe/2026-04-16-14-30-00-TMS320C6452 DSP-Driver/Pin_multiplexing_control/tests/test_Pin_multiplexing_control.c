#include <stdio.h>
#include "../include/Pin_multiplexing_control_api.h"

int main(void)
{
    int rc = Pin_multiplexing_control_feature_run();
    printf("Pin_multiplexing_control: %s (rc=%d)\n", (rc==0)?"PASS":"FAIL", rc);
    return (rc==0)?0:1;
}
