#ifndef TMS320C6452_GPIO_PIN_MULTIPLEXING_CONTROL_H
#define TMS320C6452_GPIO_PIN_MULTIPLEXING_CONTROL_H

#include <stdint.h>
#include <stdbool.h>

/* Controller Name: TMS320C6452 GPIO */
/* Base Address: UNKNOWN (pinmux belongs to device configuration module) */
/* Target Environment: CPU/Arch: UNKNOWN, OS/Baremetal: UNKNOWN, Compiler: UNKNOWN, Endianness: UNKNOWN */

/* The device pin multiplexing (PINMUX) registers are outside the GPIO module and are device/package specific.
 * This driver provides a deterministic, memory-mapped programming interface using caller-provided register
 * addresses and bitfields to select GPIO function for pins. No dynamic memory allocation is used. */

typedef struct {
    volatile uint32_t *reg;   /* Address of the PINMUX register controlling this pin/ball */
    uint32_t mask;            /* Bit mask for the function select field */
    uint32_t sel_gpio;        /* Encoded value (already shifted/masked) that selects GPIO function */
} tms320c6452_pinmux_entry_t;

/* Apply a table of pinmux entries to select GPIO function. Deterministic RMW per entry. */
void tms320c6452_pinmux_apply(const tms320c6452_pinmux_entry_t *table, uint32_t count);

/* Verify that all entries in the table are currently configured for GPIO. */
bool tms320c6452_pinmux_verify(const tms320c6452_pinmux_entry_t *table, uint32_t count);

/* Write a single entry (helper) */
static inline void tms320c6452_pinmux_write_one(const tms320c6452_pinmux_entry_t *e)
{
    uint32_t v = *e->reg;
    v &= ~e->mask;
    v |= (e->sel_gpio & e->mask);
    *e->reg = v;
}

/* Read/verify a single entry (helper) */
static inline bool tms320c6452_pinmux_is_gpio(const tms320c6452_pinmux_entry_t *e)
{
    uint32_t v = *e->reg;
    return ( (v & e->mask) == (e->sel_gpio & e->mask) );
}

#endif /* TMS320C6452_GPIO_PIN_MULTIPLEXING_CONTROL_H */
