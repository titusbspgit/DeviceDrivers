#include <stdio.h>
#include "../include/Per-pin_edge-triggered_interrupts_api.h"

int main(void)
{
    int rc = Per_pin_edge_triggered_interrupts_feature_run();
    printf("Per-pin_edge-triggered_interrupts: %s (rc=%d)\n", (rc==0)?"PASS":"FAIL", rc);
    return (rc==0)?0:1;
}
