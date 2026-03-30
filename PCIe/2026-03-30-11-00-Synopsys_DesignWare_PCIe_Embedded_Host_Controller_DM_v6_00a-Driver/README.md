Synopsys DWC PCIe DM v6.00a — PCIe 6.0 Flit Mode (64.0 GT/s) Helper
===================================================================

This package provides a minimal helper to discover and validate PCIe 6.0 Flit Mode operation on a Synopsys DesignWare PCIe Embedded Host Controller (DM) v6.00a Root Port.

Contents
- include/pcie_flit64_cfg.h: Public API and register defs
- src/pcie_flit64_cfg.c: Implementation of discovery and control helpers
- tests/pcie_flit64_test.c: Linux userspace utility to read/optionally toggle Flit Mode

Build
- Header/source are portable C (C11). For kernel integration, wrap cfg_ops with kernel PCI config accessors.
- Userspace test (Linux): `gcc -O2 -Wall -Wextra -o pcie_flit64_test tests/pcie_flit64_test.c`

Usage (userspace test)
- Run: `sudo ./pcie_flit64_test <BDF>` where BDF like `00:00.0`
- Optional: `--toggle-on` to request Flit Mode enable and retrain to 64.0 GT/s.

Notes
- The helper uses conservative assumptions for Device Control/Status 3 and Link Control 2 offsets relative to the PCIe Capability block. Actual placement follows your build’s PCIe 6.0 extended capability layout; if different, adjust macros in the header.
- Enabling Flit Mode and retraining should only be attempted when hardware and endpoints support 64.0 GT/s with Flit Mode.
- The PL64G Extended Capability (ID 0x0031) is discovered via extended capability walk for diagnostics.

License
- BSD-3-Clause
