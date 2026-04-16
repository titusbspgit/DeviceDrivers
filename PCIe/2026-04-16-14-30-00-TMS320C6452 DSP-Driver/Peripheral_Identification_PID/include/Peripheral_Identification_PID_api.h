#ifndef PID_API_H
#define PID_API_H

#include <stdint.h>
#include <stdbool.h>
#include "../../common/include/gpio_tms320c6452.h"

int Peripheral_Identification_PID_feature_run(uint32_t *pid_out);

#endif /* PID_API_H */
