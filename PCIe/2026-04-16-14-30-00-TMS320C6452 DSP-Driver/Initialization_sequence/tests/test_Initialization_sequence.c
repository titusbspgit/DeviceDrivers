#include <stdio.h>
#include "../include/Initialization_sequence_api.h"

int main(void)
{
    int rc = Initialization_sequence_feature_run();
    printf("Initialization_sequence: %s (rc=%d)\n", (rc==0)?"PASS":"FAIL", rc);
    return (rc==0)?0:1;
}
