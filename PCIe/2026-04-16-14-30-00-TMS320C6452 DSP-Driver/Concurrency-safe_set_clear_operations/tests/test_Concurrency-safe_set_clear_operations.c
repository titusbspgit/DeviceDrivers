#include <stdio.h>
#include "../include/Concurrency-safe_set_clear_operations_api.h"

int main(void)
{
    int rc = Concurrency_safe_set_clear_operations_feature_run();
    printf("Concurrency-safe_set_clear_operations: %s (rc=%d)\n", (rc==0)?"PASS":"FAIL", rc);
    return (rc==0)?0:1;
}
