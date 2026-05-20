#ifndef TMS320C6452_GPIO_BANKED_ARCHITECTURE_32_SIGNALS_2_BANKS_DRV_H
#define TMS320C6452_GPIO_BANKED_ARCHITECTURE_32_SIGNALS_2_BANKS_DRV_H

#include <stdint.h>
#include <stdbool.h>

/* Controller/Base Configuration */
#define TMS320C6452_GPIO_BASE                 (0x18000000u)

/* Register Offsets (base-relative, 32-bit) */
#define GPIO_PID_OFS                          (0x00u)
#define GPIO_BINTEN_OFS                       (0x08u)
#define GPIO_DIR01_OFS                        (0x10u)
#define GPIO_OUT_DATA01_OFS                   (0x14u)
#define GPIO_SET_DATA01_OFS                   (0x18u)
#define GPIO_CLR_DATA01_OFS                   (0x1Cu)
#define GPIO_IN_DATA01_OFS                    (0x20u)
#define GPIO_SET_RIS_TRIG01_OFS               (0x24u)
#define GPIO_CLR_RIS_TRIG01_OFS               (0x28u)
#define GPIO_SET_FAL_TRIG01_OFS               (0x2Cu)
#define GPIO_CLR_FAL_TRIG01_OFS               (0x30u)
#define GPIO_INTSTAT01_OFS                    (0x34u)

/* Bit helpers */
#define GPIO_BIT(n)                           (1u << (uint32_t)(n))

/* BINTEN bits */
#define GPIO_BINTEN_EN0                       (1u << 0)
#define GPIO_BINTEN_EN1                       (1u << 1)

/* Resets */
#define TMS320C6452_GPIO_BINTEN_RESET         (0x00000000u)
/* DIR01 reset: 0xFFFFFFFFu (inputs) */
/* OUT_DATA01 reset: 0x00000000u */
/* Edge enable resets: 0x00000000u */

#define TMS320C6452_GPIO_PIN_COUNT            (32u)
#define TMS320C6452_GPIO_BANK_COUNT           (2u)

#ifdef __cplusplus
extern "C" {
#endif

/* Status codes */
#define TMS320C6452_GPIO_OK                   (0)
#define TMS320C6452_GPIO_ERR_INVAL            (-22)

/* API: Initialization for banked architecture */
int tms320c6452_gpio_banked_architecture_init(void);

/* API: Direction control (input=true, output=false) */
int tms320c6452_gpio_set_dir(uint32_t pin, bool input);
int tms320c6452_gpio_get_dir(uint32_t pin, bool *input);

/* API: Data */
int tms320c6452_gpio_write(uint32_t pin, bool high);
int tms320c6452_gpio_read(uint32_t pin, bool *high);

/* API: Edge interrupt configuration per pin */
int tms320c6452_gpio_enable_edge(uint32_t pin, bool rising, bool falling);
int tms320c6452_gpio_disable_edge(uint32_t pin, bool rising, bool falling);

/* API: Bank interrupt enable (0 or 1) */
int tms320c6452_gpio_bank_int_enable(uint32_t bank, bool enable);

/* API: Interrupt status read (per-pin) */
uint32_t tms320c6452_gpio_int_status(void);

/* API: ISR hook */
void tms320c6452_gpio_isr(void);

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_GPIO_BANKED_ARCHITECTURE_32_SIGNALS_2_BANKS_DRV_H */
