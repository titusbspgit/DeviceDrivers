#include <stdio.h>
#include "../include/Edge_detection_synchronized_to_GPIO_clock_api.h"

int main(void)
{
    int rc = Edge_detection_synchronized_to_GPIO_clock_feature_run();
    printf("Edge_detection_synchronized_to_GPIO_clock: %s (rc=%d)\n", (rc==0)?"PASS":"FAIL", rc);
    return (rc==0)?0:1;
}
