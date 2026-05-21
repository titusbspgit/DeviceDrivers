#ifndef TMS320C6452_GPIO_CORE_H
#define TMS320C6452_GPIO_CORE_H

/* Target Environment: arch=arm, env=uboot, compiler=gcc, endianness=big */

#include <stdint.h>
#include <stdbool.h>

/* Base address
 * Source: SPRUF95 (GPIO User's Guide) for TMS320C6452.
 */
#ifndef TMS320C6452_GPIO_BASE
#define TMS320C6452_GPIO_BASE   (0x01E26000u)
#endif

/* Register Offsets */
#define GPIO_PID_OFFSET             (0x00u)
#define GPIO_BINTEN_OFFSET          (0x08u)
#define GPIO_DIR01_OFFSET           (0x10u)
#define GPIO_OUT_DATA01_OFFSET      (0x14u)
#define GPIO_SET_DATA01_OFFSET      (0x18u)
#define GPIO_CLR_DATA01_OFFSET      (0x1Cu)
#define GPIO_IN_DATA01_OFFSET       (0x20u)
#define GPIO_SET_RIS_TRIG01_OFFSET  (0x24u)
#define GPIO_CLR_RIS_TRIG01_OFFSET  (0x28u)
#define GPIO_SET_FAL_TRIG01_OFFSET  (0x2Cu)
#define GPIO_CLR_FAL_TRIG01_OFFSET  (0x30u)
#define GPIO_INTSTAT01_OFFSET       (0x34u)

/* Bit macros */
#ifndef BIT
#define BIT(n) (1u << (n))
#endif

/* Bank enable bits in BINTEN */
#define GPIO_BINTEN_EN0   BIT(0)
#define GPIO_BINTEN_EN1   BIT(1)

/* Core API that feature drivers will use */
typedef struct tms320c6452_gpio_core_api_s {
    void     (*write_reg)(uint32_t offset, uint32_t val);
    uint32_t (*read_reg)(uint32_t offset);
    void     (*set_bits)(uint32_t offset, uint32_t mask);
    void     (*clear_bits)(uint32_t offset, uint32_t mask);
} tms320c6452_gpio_core_api_t;

/* Retrieve a pointer to the statically-defined core API. */
const tms320c6452_gpio_core_api_t* tms320c6452_gpio_core_get(void);

#endif /* TMS320C6452_GPIO_CORE_H */
