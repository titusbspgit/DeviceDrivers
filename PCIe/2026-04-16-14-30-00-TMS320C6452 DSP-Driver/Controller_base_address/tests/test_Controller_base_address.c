#include <stdio.h>
#include "../include/Controller_base_address_api.h"

int main(void)
{
    int rc = Controller_base_address_feature_run();
    printf("Controller_base_address: %s (rc=%d)\n", (rc==0)?"PASS":"FAIL", rc);
    return (rc==0)?0:1;
}
