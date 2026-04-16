# Feature → Driver/API Mapping (TMS320C6452 DSP GPIO)

Base: 0xA1008000; Arch: arm; Env: Baremetal; Compiler: gcc; Endian: bigendian

Features implemented serially from feature list:
1. GPIO banked architecture (32 signals, 2 banks)
   - Represented by 32-bit banked registers; APIs use pin [0..31] → bit mapping.
2. Pin multiplexing control
   - Precondition handled via platform hooks: tmsgpio_platform_configure_pinmux().
3. Endianness independence
   - HAL provides endian-safe access; GPIO itself is endian-agnostic.
4. Direction control (DIR01)
   - APIs: tmsgpio_config_pin(), tmsgpio_config_mask().
5. Output drive control (OUT/SET/CLR)
   - APIs: tmsgpio_write_pin(), tmsgpio_set_mask(), tmsgpio_clear_mask().
6. Input sampling (IN_DATA01)
   - APIs: tmsgpio_read_pin(), tmsgpio_read_port().
7. Per-pin edge-triggered interrupts
   - API: tmsgpio_irq_config_edge().
8. Per-bank interrupt enable (BINTEN)
   - API: tmsgpio_irq_bank_enable().
9. Interrupt status and W1C (INTSTAT01)
   - APIs: tmsgpio_irq_status(), tmsgpio_irq_clear().
10. EDMA event support from GPIO
    - Routed via BINTEN per-bank enables (no extra API; covered by tmsgpio_irq_bank_enable()).
11. Clock gating and frequency (PSC/LPSC)
    - Platform hooks: tmsgpio_platform_enable_psc()/disable_psc().
12. Low-power behavior via PSC
    - APIs: tmsgpio_enter_low_power(), tmsgpio_exit_low_power().
13. Reset behavior (software vs hardware)
    - Initialization clears INTSTAT and expects DIR reset to 0xFFFFFFFF. Application should consider reset source.
14. Initialization sequence
    - API: tmsgpio_init() with platform init option.
15. Emulation suspend behavior
    - Documented behavior; no specific API (driver remains functional when CPU halts).
16. Peripheral Identification (PID)
    - API: tmsgpio_get_pid().
17. Concurrency-safe set/clear operations
    - APIs: tmsgpio_set_mask(), tmsgpio_clear_mask().
18. Edge detection synchronized to GPIO clock
    - Timing note; no specific API.
19. Open-drain interaction (readback behavior)
    - Note: IN_DATA01 reflects wired-logic; driver read APIs return synchronized state.
20. No interrupt multiplexing with other functions
    - Assumed; integrate with CPU IRQ controller as per platform.
21. Register map and offsets
    - See gpio_regs.h.
22. Controller base address
    - Default 0xA1008000; override via tmsgpio_init().
23. Error handling features
    - Defensive parameter checks; returns negative error codes.
24. Security features
    - No IP-level security; restrict MMIO region at system level.

Design notes and assumptions
- BINTEN[EN0/EN1] gate both CPU IRQ and EDMA events.
- DIR01 reset is 0xFFFFFFFF (all inputs) as per spec.
- W1C semantics on INTSTAT01 honored by tmsgpio_irq_clear().
- Endianness conversions reside in HAL; for big-endian targets, conversions are nops.
- Tests use MMIO mock and do not model all HW side effects (e.g., SET/CLR mirroring OUT).

Quality
- C99, -Wall -Wextra -Werror clean
- No dynamic memory; deterministic volatile MMIO
- MISRA-inspired checks: no unchecked nulls, bounds checks on pins
