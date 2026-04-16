#include <stdio.h>
#include "../include/Error_handling_features_api.h"

int main(void)
{
    int rc = Error_handling_features_feature_run();
    printf("Error_handling_features: %s (rc=%d)\n", (rc==0)?"PASS":"FAIL", rc);
    return (rc==0)?0:1;
}
