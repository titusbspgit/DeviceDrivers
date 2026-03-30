# Synopsys DesignWare PCIe Embedded Host Controller (DM) v6.00a — Feature Drivers

This package contains standalone, HAL-style drivers and test applications for:
- PCIe 6.0 64.0 GT/s Flit Mode
- PIPE PHY Interface (external/custom PHY via viewport)
- Secondary PCIe Extended Capability (SPCIE)
- Link Training and Equalization Controls
- ASPM L0s/L1 Control

Design constraints:
- Memory-mapped I/O only
- Deterministic, single 32-bit register accesses
- No dynamic memory allocation
- Portable C (C11), userspace-friendly tests using static mock MMIO regions

See each feature_*/include and feature_*/test for API and tests.
