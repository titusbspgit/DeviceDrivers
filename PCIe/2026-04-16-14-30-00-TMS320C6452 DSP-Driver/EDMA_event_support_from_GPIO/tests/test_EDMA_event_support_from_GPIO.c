#include <stdio.h>
#include "../include/EDMA_event_support_from_GPIO_api.h"

int main(void)
{
    int rc = EDMA_event_support_from_GPIO_feature_run();
    printf("EDMA_event_support_from_GPIO: %s (rc=%d)\n", (rc==0)?"PASS":"FAIL", rc);
    return (rc==0)?0:1;
}
