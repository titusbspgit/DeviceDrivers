#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

// Global configuration provided for driver generation
#define CONTROLLER_NAME   "TMS320C6452 DSP"
#define ARCH              "arm"
#define ENVIRONMENT       "Baremetal"
#define COMPILER_ID       "gcc"
#define ENDIANNESS_NAME   "bigendian"

// Assumed base location for example mapping (may be adapted by integrator)
#define GPIO_BASE_ADDR    (0xA1008000u)

#endif // BOARD_CONFIG_H
