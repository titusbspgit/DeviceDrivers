# PCIe 6.0 Flit Mode

- Detects PL64G extended capability (ID 0x0031) and reads version/status.
- Checks Device Capabilities 3 for FLIT support, enables via Link Control 3, and polls Status 2 for active indication.
- API: pcie_flit_query, pcie_flit_enable(timeout), pcie_flit_status.
- Sequencing: Recommend quiescing link traffic prior to enabling; on timeout, revert changes.
