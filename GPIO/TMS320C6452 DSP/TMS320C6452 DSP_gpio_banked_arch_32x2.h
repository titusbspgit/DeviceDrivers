#ifndef TMS320C6452_DSP_GPIO_BANKED_ARCH_32X2_H
#define TMS320C6452_DSP_GPIO_BANKED_ARCH_32X2_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/* Core ops header provided by core driver */
#include "TMS320C6452 DSP_core.h"

/* Register offsets relative to core base (TMS320C6452 GPIO) */
#define GPIO_REG_BINTEN_OFFSET         (0x08u)
#define GPIO_REG_DIR01_OFFSET          (0x10u)
#define GPIO_REG_OUT_DATA01_OFFSET     (0x14u)
#define GPIO_REG_SET_DATA01_OFFSET     (0x18u)
#define GPIO_REG_CLR_DATA01_OFFSET     (0x1Cu)
#define GPIO_REG_IN_DATA01_OFFSET      (0x20u)
#define GPIO_REG_SET_RIS_TRIG01_OFFSET (0x24u)
#define GPIO_REG_CLR_RIS_TRIG01_OFFSET (0x28u)
#define GPIO_REG_SET_FAL_TRIG01_OFFSET (0x2Cu)
#define GPIO_REG_CLR_FAL_TRIG01_OFFSET (0x30u)
#define GPIO_REG_INTSTAT01_OFFSET      (0x34u)

/* Bank mapping: bank 0 => bits [15:0]; bank 1 => bits [31:16] */
#define GPIO_BANK0_MASK   (0x0000FFFFu)
#define GPIO_BANK1_MASK   (0xFFFF0000u)

/* Error codes */
#define GPIO_EOK               (0)
#define GPIO_EINVAL            (-22)

/* API declarations */
int32_t gpio_arch_init(const struct gpio_core_ops* ops);
int32_t gpio_set_dir_bank(uint32_t bank, uint32_t mask, bool output);
int32_t gpio_write_bank(uint32_t bank, uint32_t set_mask, uint32_t clr_mask);
int32_t gpio_read_bank(uint32_t bank, uint32_t* in_bits);
int32_t gpio_irq_enable_bank(uint32_t bank, uint32_t mask, bool enable);
int32_t gpio_irq_status_bank(uint32_t bank, uint32_t* status);

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_DSP_GPIO_BANKED_ARCH_32X2_H */
