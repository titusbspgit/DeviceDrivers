TMS320C6452 DSP GPIO Driver (Bare-metal, Big-endian)

This drop contains:
- common/: Production-quality GPIO driver (header and source) for TMS320C6452 at base 0xA1008000
- Per-feature subfolders with wrapper APIs, tests, and Makefiles for core GPIO features

Build example:
$ cd PCIe/2026-04-16-14-30-00-TMS320C6452\ DSP-Driver/Direction_control_DIR01
$ make
$ ./test_Direction_control_DIR01

Notes:
- Tests assume a bare-metal harness providing stdout; HAL clock/pinmux stubs default to success.
- All MMIO is via 32-bit accesses; endianness-independent per peripheral spec.
