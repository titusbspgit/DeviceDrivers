#include <stdio.h>
#include "../include/Register_map_and_offsets_api.h"

int main(void)
{
    int rc = Register_map_and_offsets_feature_run();
    printf("Register_map_and_offsets: %s (rc=%d)\n", (rc==0)?"PASS":"FAIL", rc);
    return (rc==0)?0:1;
}
