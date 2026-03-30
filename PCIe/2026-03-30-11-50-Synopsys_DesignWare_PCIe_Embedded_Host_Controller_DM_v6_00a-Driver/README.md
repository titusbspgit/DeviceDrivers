Synopsys DWC PCIe DM v6.00a — PCIe 6.0 Flit Mode (64.0 GT/s) Helper (single-commit)
===================================================================================

This package provides a helper to discover/enable/verify PCIe 6.0 Flit Mode on a Synopsys DesignWare PCIe Embedded Host Controller (DM) v6.00a Root Port.

Key improvements vs previous drop
- Robust, spec-aligned offsets for DevCtl3/DevSts3 (+0x38) and LnkCtl2/LnkSts2 (+0x30) relative to PCIe Capability base
- Polling with timeout for link retrain completion, DLLLA, and Flit Active at 64.0 GT/s
- PL64G (ID 0x0031) STATUS dump in test for diagnostics
- Code cleanup: removed unused helpers/macros; capability-driven checks

Build (userspace test)
- gcc -O2 -Wall -Wextra -o pcie_flit64_test tests/pcie_flit64_test.c

Usage
- sudo ./pcie_flit64_test <BDF> [--toggle-on]

Integration notes
- Provide struct pcie_cfg_ops for kernel/RTOS usage to invoke the helper APIs.
- Sequence: init → find_capabilities → get_status; to enable: request_enable(true, true, timeout_us)

License
- BSD-3-Clause
