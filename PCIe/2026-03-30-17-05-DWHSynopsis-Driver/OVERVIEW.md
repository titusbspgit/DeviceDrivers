# DesignWare PCIe (DM v6.00a) Feature Drivers (ARM big-endian)

This package provides production-quality, per-feature drivers for the Synopsys DesignWare PCIe Embedded Host Controller (DM v6.00a), targeting ARM big-endian, bare-metal environments.

Features included:
- PCIe 6.0 Flit Mode control and status
- External PIPE PHY access via DWC PHY viewport
- Secondary PCIe Extended Capability (Link Control 3 / Lane Error Status)
- Link Training and Equalization controls (Gen3+)
- ASPM L0s/L1 policy and vendor timing knobs

Structure:
- drivers/pcie/common: Endian-safe HAL, capability discovery
- drivers/pcie/features/<feature>: Self-contained APIs and docs
- tests/pcie/<feature>: RAM-backed DBI mock tests
- examples/pcie/<feature>: Minimal usage examples

Build:
- make tests (host): builds unit tests with PCIE_HAL_HOST_TEST=1 to avoid byte swapping
- make all (target): builds libraries for ARM big-endian with -ffreestanding

Safety/MISRA:
- No dynamic memory; fixed-width types; volatile-qualified MMIO; error enums; bounded loops and timeouts.

Integration:
- Link the common HAL and the required feature modules. Provide DBI base mapping from your SoC platform code. Adjust macros in headers if your synthesized bit positions differ (e.g., PORT_FORCE, LINK_CTRL3).
