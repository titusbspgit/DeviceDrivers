#include <stdio.h>
#include "../include/Security_features_api.h"

int main(void)
{
    int rc = Security_features_feature_run();
    printf("Security_features: %s (rc=%d)\n", (rc==0)?"PASS":"FAIL", rc);
    return (rc==0)?0:1;
}
