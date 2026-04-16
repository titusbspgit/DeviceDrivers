#include <stdio.h>
#include "../include/Endianness_independence_api.h"

int main(void)
{
    int rc = Endianness_independence_feature_run();
    printf("Endianness_independence: %s (rc=%d)\n", (rc==0)?"PASS":"FAIL", rc);
    return (rc==0)?0:1;
}
