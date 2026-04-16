#ifndef FEATURE_REGMAP_H
#define FEATURE_REGMAP_H

#include <stdint.h>
#include <stdbool.h>

#define GPIO_REG_PID_OFFSET              (0x00u)
#define GPIO_REG_BINTEN_OFFSET           (0x08u)
#define GPIO_REG_DIR01_OFFSET            (0x10u)
#define GPIO_REG_OUT_DATA01_OFFSET       (0x14u)
#define GPIO_REG_SET_DATA01_OFFSET       (0x18u)
#define GPIO_REG_CLR_DATA01_OFFSET       (0x1Cu)
#define GPIO_REG_IN_DATA01_OFFSET        (0x20u)
#define GPIO_REG_SET_RIS_TRIG01_OFFSET   (0x24u)
#define GPIO_REG_CLR_RIS_TRIG01_OFFSET   (0x28u)
#define GPIO_REG_SET_FAL_TRIG01_OFFSET   (0x2Cu)
#define GPIO_REG_CLR_FAL_TRIG01_OFFSET   (0x30u)
#define GPIO_REG_INTSTAT01_OFFSET        (0x34u)

#endif /* FEATURE_REGMAP_H */
