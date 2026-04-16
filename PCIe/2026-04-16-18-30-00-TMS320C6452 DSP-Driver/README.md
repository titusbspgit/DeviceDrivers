# TMS320C6452 DSP GPIO Driver (Big-endian, Baremetal)

- Controller: TMS320C6452 DSP
- Base: 0xA1008000
- Arch: arm
- Env: Baremetal
- Compiler: gcc
- Endianness: bigendian

## Layout
```
PCIe/2026-04-16-18-30-00-TMS320C6452 DSP-Driver/
  drivers/tms320c6452/hal/        # Endian-safe MMIO accessors
  drivers/tms320c6452/gpio/       # GPIO driver and registers
  tests/tms320c6452/              # Host test with MMIO mock
  Makefile                        # Host test build (gcc)
  docs/Feature_Drivers.md         # Features and API mapping
```

## Build (host tests)
- make tests
- Run: ./PCIe/2026-04-16-18-30-00-TMS320C6452\ DSP-Driver/build/gpio_test

## Notes
- HAL implements big-endian MMIO semantics; test uses MMIO mock and works on little-endian hosts.
- Platform-specific PSC and Pinmux are provided as weak hooks in the driver and should be implemented by the board code on target.
