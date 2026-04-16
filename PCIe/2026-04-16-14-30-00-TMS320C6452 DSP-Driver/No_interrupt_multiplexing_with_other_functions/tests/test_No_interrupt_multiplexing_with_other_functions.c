#include <stdio.h>
#include "../include/No_interrupt_multiplexing_with_other_functions_api.h"

int main(void)
{
    int rc = No_interrupt_multiplexing_with_other_functions_feature_run();
    printf("No_interrupt_multiplexing_with_other_functions: %s (rc=%d)\n", (rc==0)?"PASS":"FAIL", rc);
    return (rc==0)?0:1;
}
