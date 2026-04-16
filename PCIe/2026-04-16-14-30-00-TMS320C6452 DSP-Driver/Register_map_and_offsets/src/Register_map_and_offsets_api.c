#include "../include/Register_map_and_offsets_api.h"

int Register_map_and_offsets_feature_run(void)
{
    /* Sanity-check: key offsets are distinct and aligned */
    if ((GPIO_REG_PID_OFFSET != 0x00u) || (GPIO_REG_DIR01_OFFSET == GPIO_REG_IN_DATA01_OFFSET)) { return -1; }
    if (((GPIO_REG_DIR01_OFFSET % 4u) != 0u) || ((GPIO_REG_INTSTAT01_OFFSET % 4u) != 0u)) { return -2; }
    return 0;
}
