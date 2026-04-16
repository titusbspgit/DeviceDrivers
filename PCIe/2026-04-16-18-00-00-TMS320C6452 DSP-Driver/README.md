# TMS320C6452 DSP GPIO Driver (Big-endian, Baremetal)

SPDX-License-Identifier: MIT

This package provides a production-quality GPIO driver for the TI TMS320C6452 DSP, targeting ARM baremetal GCC builds with big-endian operation. It includes a shared HAL for endian-safe MMIO, feature APIs aligned to the device spec, and a unit test using an MMIO mock backend.

Key parameters
- Controller: TMS320C6452 DSP
- Base address: 0xA1008000
- Arch: arm
- Env: Baremetal
- Compiler: gcc
- Endianness: bigendian

Contents
- drivers/tms320c6452/hal: endian-safe MMIO accessors
- drivers/tms320c6452/gpio: GPIO driver and registers
- tests/tms320c6452: unit tests with MMIO mock

Build (host GCC for unit tests)
- make tests
- Run: ./PCIe/2026-04-16-18-00-00-TMS320C6452\\ DSP-Driver/build/gpio_test

Porting to target
- Replace platform hook stubs in gpio.c (tmsgpio_platform_* functions) with board-specific PSC and pinmux control.
- Compile with your ARM baremetal toolchain and link into your firmware.

See docs/Feature_Drivers.md for feature mapping and API references.
