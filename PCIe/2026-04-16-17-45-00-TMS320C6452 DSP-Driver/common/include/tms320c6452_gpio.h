/*
 * TMS320C6452 DSP GPIO Driver - Header
 * RCI-ag-DrvGen Agent
 *
 * Assumptions (documented where spec was ambiguous):
 * - Register offsets chosen per common TI C64x patterns; update if official manual differs.
 * - 32-bit MMIO, big-endian CPU; GPIO IP is endian-agnostic.
 * - SET/CLR/edge-set/edge-clr are treated as write-only; driver does not rely on their readback.
 * - INTSTAT01 is write-one-to-clear (W1C).
 * - BINTEN[0]=Bank0 enable (pins 0..15), BINTEN[1]=Bank1 enable (pins 16..31).
 * - Per-pin edge detect works irrespective of DIR (per feature list).
 */
#ifndef TMS320C6452_GPIO_H
#define TMS320C6452_GPIO_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Base address (from memmap): 0xA1008000 */
#ifndef TMS_GPIO_BASE_DEFAULT
#define TMS_GPIO_BASE_DEFAULT   (0xA1008000u)
#endif

/* Register offsets (assumed; verify against device manual) */
#define TMS_GPIO_PID_OFS            (0x000u) /* Peripheral ID (RO) */
#define TMS_GPIO_BINTEN_OFS         (0x008u) /* Bank interrupt enable (RW) */
#define TMS_GPIO_DIR01_OFS          (0x010u) /* Direction: 1=input, 0=output (RW) */
#define TMS_GPIO_OUT_DATA01_OFS     (0x014u) /* Output data (RW) */
#define TMS_GPIO_SET_DATA01_OFS     (0x018u) /* Atomic set (WO) */
#define TMS_GPIO_CLR_DATA01_OFS     (0x01Cu) /* Atomic clear (WO) */
#define TMS_GPIO_IN_DATA01_OFS      (0x020u) /* Input sampled data (RO) */
#define TMS_GPIO_SET_RIS_TRIG01_OFS (0x024u) /* Rising edge enable set (WO) */
#define TMS_GPIO_CLR_RIS_TRIG01_OFS (0x028u) /* Rising edge enable clear (WO) */
#define TMS_GPIO_SET_FAL_TRIG01_OFS (0x02Cu) /* Falling edge enable set (WO) */
#define TMS_GPIO_CLR_FAL_TRIG01_OFS (0x030u) /* Falling edge enable clear (WO) */
#define TMS_GPIO_INTSTAT01_OFS      (0x034u) /* Interrupt status (R/W1C) */

/* BINTEN bit definitions */
#define TMS_GPIO_BINTEN_EN0_MASK    (0x00000001u)
#define TMS_GPIO_BINTEN_EN1_MASK    (0x00000002u)

/* Helper masks */
#define TMS_GPIO_BANK0_MASK         (0x0000FFFFu)
#define TMS_GPIO_BANK1_MASK         (0xFFFF0000u)
#define TMS_GPIO_PIN_MASK(pin)      ( (uint32_t)1u << (uint32_t)(pin) )

/* Error codes */
typedef enum {
    TMS_GPIO_OK = 0,
    TMS_GPIO_EINVAL = 1,
    TMS_GPIO_ESTATE = 2
} tms_gpio_status_t;

/* Forward declaration */
struct tms_gpio_ctx_s;

/* Optional board hooks */
typedef int (*tms_gpio_psc_ctrl_fn)(void); /* return 0 on success */
typedef int (*tms_gpio_pinmux_validate_fn)(void); /* return 0 when pins muxed for GPIO */

typedef void (*tms_gpio_pin_isr_cb_t)(void *user, uint32_t pin);

typedef struct tms_gpio_ctx_s {
    uintptr_t base; /* MMIO base */
    /* Optional hooks for power/clock and pinmux */
    tms_gpio_psc_ctrl_fn psc_enable;
    tms_gpio_psc_ctrl_fn psc_disable;
    tms_gpio_pinmux_validate_fn pinmux_validate;
    /* ISR callbacks per pin (optional) */
    tms_gpio_pin_isr_cb_t pin_cb[32];
    void *pin_cb_user[32];
    /* Internal flags */
    bool clock_enabled;
} tms_gpio_ctx_t;

/* Low-level MMIO accessors (32-bit) */
static inline void tms_gpio_reg_write(const tms_gpio_ctx_t *ctx, uint32_t ofs, uint32_t val)
{
    volatile uint32_t *addr = (volatile uint32_t *)(ctx->base + (uintptr_t)ofs);
    *addr = val; /* 32-bit write, endian-agnostic per IP */
}

static inline uint32_t tms_gpio_reg_read(const tms_gpio_ctx_t *ctx, uint32_t ofs)
{
    const volatile uint32_t *addr = (const volatile uint32_t *)(ctx->base + (uintptr_t)ofs);
    return *addr; /* 32-bit read */
}

/* Initialization and power/clock */
void tms_gpio_ctx_init(tms_gpio_ctx_t *ctx,
                       uintptr_t base,
                       tms_gpio_psc_ctrl_fn psc_enable,
                       tms_gpio_psc_ctrl_fn psc_disable,
                       tms_gpio_pinmux_validate_fn pinmux_validate);

tms_gpio_status_t tms_gpio_enable_module(tms_gpio_ctx_t *ctx);

tms_gpio_status_t tms_gpio_disable_module(tms_gpio_ctx_t *ctx);

/* PID and identification */
uint32_t tms_gpio_get_pid(const tms_gpio_ctx_t *ctx);

/* Direction control */
tms_gpio_status_t tms_gpio_set_direction(tms_gpio_ctx_t *ctx, uint32_t pin, bool input);

tms_gpio_status_t tms_gpio_get_direction(const tms_gpio_ctx_t *ctx, uint32_t pin, bool *input_out);

/* Output control (SET/CLR preferred) */
tms_gpio_status_t tms_gpio_write_pin(tms_gpio_ctx_t *ctx, uint32_t pin, bool level_high);

tms_gpio_status_t tms_gpio_set_mask(tms_gpio_ctx_t *ctx, uint32_t mask);

tms_gpio_status_t tms_gpio_clear_mask(tms_gpio_ctx_t *ctx, uint32_t mask);

/* Input sampling */
uint32_t tms_gpio_read_inputs(const tms_gpio_ctx_t *ctx);

/* Per-pin edge-triggered interrupt configuration */
tms_gpio_status_t tms_gpio_config_edge(tms_gpio_ctx_t *ctx, uint32_t pin, bool rising_en, bool falling_en);

/* Per-bank interrupt enable (BINTEN) */
tms_gpio_status_t tms_gpio_enable_bank_irq(tms_gpio_ctx_t *ctx, uint32_t bank, bool enable);

/* Interrupt status (read and clear specific bits via W1C) */
uint32_t tms_gpio_get_irq_status(const tms_gpio_ctx_t *ctx);
void tms_gpio_clear_irq_status(tms_gpio_ctx_t *ctx, uint32_t mask);

/* ISR handling: register per-pin callback and service bank */
tms_gpio_status_t tms_gpio_register_pin_isr(tms_gpio_ctx_t *ctx, uint32_t pin, tms_gpio_pin_isr_cb_t cb, void *user);
void tms_gpio_service_bank_isr(tms_gpio_ctx_t *ctx, uint32_t bank);

/* Low-power helpers (quiesce interrupts; power hooks) */
tms_gpio_status_t tms_gpio_quiesce_interrupts(tms_gpio_ctx_t *ctx);
tms_gpio_status_t tms_gpio_enter_low_power(tms_gpio_ctx_t *ctx);
tms_gpio_status_t tms_gpio_exit_low_power(tms_gpio_ctx_t *ctx);

/* Feature-tagged helper APIs (one per feature for traceability) */
/* 1. Banked architecture info */
uint32_t tms_gpio_feature_banked_arch_get_bank_mask(uint32_t bank);
/* 2. Pinmux validate wrapper */
tms_gpio_status_t tms_gpio_feature_pinmux_validate(tms_gpio_ctx_t *ctx);
/* 3. Endianness independence: no-op, always OK */
tms_gpio_status_t tms_gpio_feature_endianness_check(void);
/* 4. Direction control: set/get wrappers already exist */
/* 5. Output drive control: wrappers exist */
/* 6. Input sampling: wrapper exists */
/* 7. Per-pin edge-triggered interrupts: wrapper exists */
/* 8. Per-bank interrupt enable: wrapper exists */
/* 9. INTSTAT W1C: wrappers exist */
/* 10. EDMA event support: bank enable is the gate; caller configures EDMA at SoC-level */
/* 11&12. PSC clock/low power controlled via hooks */
/* 13. Reset behavior helpers */
void tms_gpio_feature_reset_reinit_sequence(tms_gpio_ctx_t *ctx);
/* 14. Initialization sequence helper */
tms_gpio_status_t tms_gpio_feature_full_init(tms_gpio_ctx_t *ctx);
/* 15. Emulation suspend: no driver action */
/* 16. PID access provided */
/* 17. Concurrency-safe set/clear: use SET/CLR functions */
/* 18. Edge detect synchronized: no config, document */
/* 19. Open-drain interaction: read_inputs reflects wired-OR level */
/* 20. No shared vectors: not applicable to driver code here */
/* 21/22. Register map and base: defined in this header */
/* 23. Error handling: status codes used */
/* 24. Security: enforced at system level */

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_GPIO_H */
