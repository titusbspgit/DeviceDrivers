# TMS320C6452 DSP — PCIe Controller Drivers (Assumed DWC-like)

This folder contains production-oriented, bare-metal C drivers targeting an ARM big-endian environment for a PCIe-like controller base-mapped at 0xA1008000 on TMS320C6452 DSP platforms. Each feature is split into its own module with a small test application.

IMPORTANT: Exact register map must be verified from the official SoC/IP specification. Where ambiguity exists, conservative assumptions were used. Replace include/pcie_regs.h with the precise map once available.

- Endianness: CPU big-endian; MMIO helpers provide optional byte-swap for LE-mapped controllers.
- No dynamic allocation; deterministic volatile MMIO.
- Toolchain: gcc/arm-none-eabi-gcc; Bare-metal.

Features:
- CORE: enable, soft reset, ID read, ready check
- LINK: LTSSM enable/start, link-up poll, status read
- CFG: Type0/1 style configuration space read/write (DWORD window)
- MSI: enable/program address+data
- INTR: mask/unmask, status read, W1C clear
- DMA: stub memcpy engine (spec-dependent)

Build:
- make (produces test_*.elf) — adjust CROSS, flags, and link script as per your platform.

Tests:
- Each test is self-contained. They perform simple sequences and print basic status.

Limitations/TODO:
- Replace assumed register offsets/masks with official values.
- Provide proper startup/runtime (crt0, linker script) for on-target execution.
- Complete DMA engine details (descriptor formats, burst, interrupts).
