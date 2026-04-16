#include <stdio.h>
#include <stdint.h>
#include "../include/Peripheral_Identification_PID_api.h"

int main(void)
{
    uint32_t pid=0U; int rc = Peripheral_Identification_PID_feature_run(&pid);
    printf("PID=0x%08X expected 0x%08X => %s\n", (unsigned)pid, (unsigned)GPIO_PID_RESET_VALUE, (rc==0)?"PASS":"FAIL");
    return (rc==0)?0:1;
}
