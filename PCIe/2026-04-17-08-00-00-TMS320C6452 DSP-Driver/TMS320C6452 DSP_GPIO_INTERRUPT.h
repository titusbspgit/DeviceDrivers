/* RCI-ag-DrvGen Agent: GPIO Interrupt driver header for TMS320C6452 DSP */
#ifndef TMS320C6452_DSP_GPIO_INTERRUPT_H
#define TMS320C6452_DSP_GPIO_INTERRUPT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/* Register offsets (from RAG) */
#define GPIO_BINTEN_OFFSET          (0x08u)
#define GPIO_SET_RIS_TRIG01_OFFSET  (0x24u)
#define GPIO_CLR_RIS_TRIG01_OFFSET  (0x28u)
#define GPIO_SET_FAL_TRIG01_OFFSET  (0x2Cu)
#define GPIO_CLR_FAL_TRIG01_OFFSET  (0x30u)
#define GPIO_INTSTAT01_OFFSET       (0x34u) /* R/W1C */

#define GPIO_PIN_MASK(pin_)         ((uint32_t)1u << (uint32_t)(pin_))
#define GPIO_MAX_PIN                (31u)

/* IRQ type selection (edge-only per RAG) */
typedef enum {
    GPIO_IRQ_LEVEL_LOW = 0,  /* not supported: mapped to falling edge + invert note */
    GPIO_IRQ_LEVEL_HIGH,     /* not supported: mapped to rising edge + invert note */
    GPIO_IRQ_EDGE_FALL,
    GPIO_IRQ_EDGE_RISE,
    GPIO_IRQ_EDGE_BOTH
} gpio_irq_type;

/* Status codes */
typedef enum {
    GPIO_IRQ_OK = 0,
    GPIO_IRQ_ERR_PARAM = -1
} gpio_irq_status_t;

/* API */
void gpio_irq_init(uintptr_t base);

gpio_irq_status_t gpio_irq_config(uint32_t pin, gpio_irq_type type);

gpio_irq_status_t gpio_irq_enable(uint32_t pin);

gpio_irq_status_t gpio_irq_disable(uint32_t pin);

gpio_irq_status_t gpio_irq_status(uint32_t pin, bool * pending);

gpio_irq_status_t gpio_irq_clear(uint32_t pin);

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_DSP_GPIO_INTERRUPT_H */
