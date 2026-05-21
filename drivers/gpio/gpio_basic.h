#ifndef GPIO_BASIC_H
#define GPIO_BASIC_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// Generic, non-soC-specific GPIO driver for TMS320C6452-class GPIO block.
// No hard-coded offsets: all register offsets are supplied by the integrator.
// Endianness-safe accesses are performed via ag_be_io.h helpers.

typedef void (*gpio_lpsc_enable_fn)(void);

typedef struct {
    uintptr_t base;           // Base address of GPIO register block
    uint32_t dir_off;         // DIR register offset
    uint32_t set_data_off;    // SET_DATA register offset
    uint32_t clr_data_off;    // CLR_DATA register offset
    uint32_t outdata_off;     // OUTDATA register offset (readback)
    uint32_t indata_off;      // INDATA register offset (synchronized input)
    gpio_lpsc_enable_fn lpsc_enable; // Optional: power/clock enable hook (may be NULL)
} gpio_basic_t;

// Initialize GPIO: enables clock via LPSC hook (if provided) and sets initial directions/levels to safe defaults.
void gpio_basic_init(const gpio_basic_t *ctx);

// Configure pin direction: true = output, false = input. Pins are 0..31.
void gpio_set_dir(const gpio_basic_t *ctx, uint32_t pin, bool is_output);

// Write logic level to an output pin. For safety, does not change DIR.
void gpio_write(const gpio_basic_t *ctx, uint32_t pin, bool high);

// Read logic level from a pin (uses INDATA).
bool gpio_read(const gpio_basic_t *ctx, uint32_t pin);

#ifdef __cplusplus
}
#endif

#endif // GPIO_BASIC_H
