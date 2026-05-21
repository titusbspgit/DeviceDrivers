#ifndef GPIO_IRQ_H
#define GPIO_IRQ_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*gpio_irq_handler_t)(uint32_t pin, void *user);

typedef bool (*irq_register_fn)(int irq_num, void (*isr)(void *), void *arg);

typedef struct {
    uintptr_t base;            // Base address of GPIO block
    uint32_t set_ris_off;      // SET_RIS_TRIG
    uint32_t clr_ris_off;      // CLR_RIS_TRIG
    uint32_t set_fal_off;      // SET_FAL_TRIG
    uint32_t clr_fal_off;      // CLR_FAL_TRIG
    uint32_t instat_off;       // INSTAT (per-pin pending)
    uint32_t intstat01_off;    // INTSTAT01 (banked status)
    int direct_irq_num;        // CPU IRQ number if direct GPIO IRQ is used (UNKNOWN -> -1)
    irq_register_fn irq_register; // Platform hook to register ISR (may be NULL)
    gpio_irq_handler_t cb;     // Client callback on edge
    void *cb_user;             // Opaque user pointer
} gpio_irq_t;

// Configure per-pin rising/falling edge enables.
void gpio_irq_configure(const gpio_irq_t *ctx, uint32_t pin, bool rising, bool falling);

// Clear per-pin latched edge(s) in INSTAT.
void gpio_irq_clear(const gpio_irq_t *ctx, uint32_t pin);

// Read banked interrupt status (INTSTAT01) to identify sources when banked IRQ is used.
uint32_t gpio_irq_bank_status(const gpio_irq_t *ctx);

// Install ISR via platform IRQ hook (no effect if irq_register is NULL or direct_irq_num < 0).
bool gpio_irq_install_isr(gpio_irq_t *ctx);

#ifdef __cplusplus
}
#endif

#endif // GPIO_IRQ_H
