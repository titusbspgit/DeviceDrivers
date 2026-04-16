TMS320C6452 GPIO Bare-metal Driver
==================================

Overview
- Controller: TMS320C6452 GPIO (32 pins, 2 banks)
- Base address: 0xA1008000
- Endianness: Big-endian (GPIO accesses are endian-independent; 32-bit MMIO)
- Environment: Bare-metal, GCC

Features implemented
- Initialization and PID readout
- Direction control per pin (DIR01)
- Output drive via atomic SET_DATA01 / CLR_DATA01 and bulk OUT_DATA01 through masks
- Input sampling via IN_DATA01
- Per-pin edge-triggered interrupts (rising/falling/both)
- Per-bank interrupt enable (BINTEN)
- Interrupt status read and W1C clear (INTSTAT01)
- HAL hooks for PSC clock enable and pinmux (weak defaults)

Build
- Make sure your GCC toolchain targets big-endian ARM as configured in your environment.
- Run: make

Run
- On your target or simulator environment, load gpio_test.elf.
- The test prints results to stdout (adjust for your platform I/O).

HAL integration
- Provide platform-specific implementations of:
  - bool gpio_hal_enable_clock(void);
  - bool gpio_hal_configure_pinmux(uint32_t pin);
- Link your HAL overrides ahead of this driver to replace the weak defaults.

Safety and assumptions
- All MMIO is 32-bit; reserved bits are preserved where applicable.
- No dynamic memory; deterministic register access.
- GPIO module clock must be enabled and pins muxed to GPIO before use.

File layout
- include/gpio_tms320c6452.h
- src/gpio_tms320c6452.c
- tests/test_gpio_tms320c6452.c
- Makefile
